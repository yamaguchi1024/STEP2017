#!/usr/local/bin/ruby

require 'cgi'
require 'json'

case ENV['REQUEST_METHOD']
when 'GET'
	cgi = CGI.new
	json = cgi['json']
	if json.nil? || json.empty?
		cgi.out { '<form method="get"><textarea name="json"></textarea><input type="submit"/></form>' }
		exit
	end
when 'POST'
	puts 'Content-Type: text/plain'
	puts
	json = STDIN.read.to_s
end

data = JSON.parse(json)

board = data['board']
pieces = board['Pieces']
nxt = board['Next']

me = nxt
op = nxt == 1 ? 2 : 1

pretty = pieces.flatten.join.gsub(/[012]/, { me.to_s => '1', op.to_s => '0', '0' => '*' }).scan(/.{8}/).join("\n")

system("g++ othello.cpp")
io = IO.popen("./a.out", "r+")
io.puts pretty
res = io.read

case ENV['REQUEST_METHOD']
when 'GET'
	cgi.out{res}
when 'POST'
	puts res
end
