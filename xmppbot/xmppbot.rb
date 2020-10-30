#!/usr/bin/ruby

# frozen_string_literal: true

# This script will subscribe to the specified JID for incoming
# messages.  It will execute a script based on published
# messages. Upon successful completion it will publish a message on
# the specified JID.

require 'aws-sdk-s3'
require 'json'
require 'optparse'
require 'rest-client'
require 'xmpp4r'
require 'xmpp4r/pubsub/helper/servicehelper'

# Jabber.debug = true

# settings
key = '30:cb28c17a27ced8b9627e7ee3397735e9'
auth = "Metaspace-Token api_key=#{key}"
url = 'http://localhost:9393'

to = nil
incoming = nil
action = nil
workdir = nil
outgoing = nil
OptionParser.new do |opts|
  opts.banner = 'Usage: xmppbot -t pubsub.xmpp.metatooth.com\
 -i \'500 Mouthguard Export\' -s ./mouthguard.sh -w ~/metaspace/PROD\
 -o \'600 Mouthguard PrintReady\''
  opts.separator ''
  opts.on('-t', '--to PUBSUB', 'publish-subscribe domain') { |t| to = t }
  opts.on('-i', '--incoming NODE', 'incoming node') { |i| incoming = i }
  opts.on('-s', '--script BASHFILE', 'bash file to execute') { |s| action = s }
  opts.on('-w', '--workdir DIR', 'working directory') { |w| workdir = w }
  opts.on('-o', '--outgoing NODE', 'outgoing node') { |o| outgoing = o }
  opts.on_tail('-h', '--help', 'Show this message') do
    puts opts
    exit
  end
  opts.parse!(ARGV)
end

puts 'No pub-sub domain specified. See xmppbot -h' if to.nil?

puts 'No incoming node specified. See xmppbot -h' if incoming.nil?

puts 'No script action specified. See xmppbot -h' if action.nil?

puts 'No working directory specified. See xmppbot -h' if workdir.nil?

puts 'No outgoing node specified. See xmppbot -h' if outgoing.nil?

def fetch(url, path, options = nil)
  response = RestClient.get(url, options)
  File.open(path, 'w') do |f|
    f << response.body
  end
end

def fetch_data(url, path, options = nil)
  response = RestClient.get(url, options)
  json = JSON.parse(response.body)
  File.open(path, 'w') do |f|
    f << json['data']
  end
  json
end

def fetch_json(url, path, options = nil)
  response = RestClient.get(url, options)
  json = JSON.parse(response.body)
  File.open(path, 'w') do |f|
    f << json
  end
  json
end

def fetch_all(url, plan, revision, dirs, auth)
  revision = fetch_json("#{url}/plans/#{plan}/revisions/#{revision}",
                        File.join(dirs['incoming'], 'revision.json'),
                        { authorization: auth })

  assimp = fetch_json(revision['data']['location'],
                      File.join(dirs['incoming'], 'plan.json'))

  fetch(assimp['meshrefs'][0]['url'], File.join(dirs['incoming'], 'mesh.stl'))
end

def make_directory(path)
  Dir.mkdir(path) unless File.directory?(path)
  path
end

def make_directories(workdir, plan, incoming, outgoing)
  resp = {}
  resp['workdir'] = make_directory(workdir)
  resp['plan'] = make_directory(File.join(resp['workdir'], plan))
  resp['incoming'] = make_directory(File.join(resp['plan'], incoming))
  resp['outgoing'] = make_directory(File.join(resp['plan'], outgoing))
  resp
end

def make_item(plan, asset)
  item = Jabber::PubSub::Item.new
  entry = REXML::Element.new('entry')

  el = REXML::Element.new('plan')
  el.text = plan
  entry.add_element(el)

  el = REXML::Element.new('asset')
  el.text = asset
  entry.add_element(el)

  item.add_element(entry)

  item
end

def make_key(path)
  digest = Digest::MD5.file(path).hexdigest
  ext = File.extname(path)
  File.join(timestamp, digest + ext)
end

def post(url, auth, key)
  body = { 'data': {
    'url': "https://#{ENV['S3_BUCKET_NAME']}.s3.amazonaws.com/#{key}",
    'mime_type': 'application/sla',
    'service': 's3',
    'bucket': ENV['S3_BUCKET_NAME'],
    's3key': key
  } }

  RestClient.post("#{url}/assets", body, { authorization: auth })
end

def timestamp
  time = Time.new
  year = time.year.to_s
  month = time.month.to_s
  day = time.day.to_s

  month = "0#{month}" if time.month < 10
  day = "0#{day}" if time.day < 10

  "#{year}/#{month}/#{day}/"
end

def upload(path, key)
  client = Aws::S3::Client.new
  File.open(path, 'rb') do |file|
    client.put_object({
                        body: file,
                        bucket: ENV['S3_BUCKET_NAME'],
                        key: key
                      })
  end
end

client = Jabber::Client.new(ENV['JABBER_ID'])
client.connect
client.auth(ENV['JABBER_PASSWORD'])

helper = Jabber::PubSub::ServiceHelper.new(client, to)
helper.subscribe_to(incoming)

items = helper.get_items_from(incoming, 3)

items.each do |_key, value|
  plan = ''
  revision = ''
  value.each_element do |elem|
    case elem.name
    when 'plan'
      plan = elem.text
    when 'revision'
      revision = elem.text
    end
  end

  puts "Processing /plans/#{plan}/revisions/#{revision} ..."
  dirs = make_directories(workdir, plan, incoming, outgoing)

  fetch_all(url, plan, revision, dirs, auth)

  value = `#{action} #{dirs['plan']}`
  puts value

  shellpath = File.join(dirs['outgoing'], 'shell.stl')
  key = make_key(shellpath)

  upload(shellpath, key)

  response = post(url, auth, key)
  json = JSON.parse(response.body)

  response = helper.publish_item_to(outgoing, make_item(plan, json['locator']))
  puts response
end
