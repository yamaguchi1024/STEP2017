require 'timeout'
require 'uri'
require 'net/http'
require 'net/https'

main = IO.popen('./main','w+')
URL = "https://icanhazwordz.appspot.com/"
uri = URI.parse(URL)

# first gets
http = Net::HTTP.new(uri.host, uri.port)
http.use_ssl = true

request = Net::HTTP::Get.new(uri.request_uri)

response = http.request(request)
html = response.body

moves = []
seed = html.scan(/name=Seed value="(.*?)"/).flatten.join("")
started = html.scan(/name=Started value="(.*?)"/).flatten.join("")
count = 0

# Started変えてみる
started += 3
req = Net::HTTP::Post.new(uri.request_uri)
req.set_form_data({'Seed' => seed, 'Started' => started}, ';')

res = Net::HTTP.new(uri.host, uri.port).tap{|h| h.use_ssl=true}.start do |http|
    response = http.request(req)
    html = response.body
end
# おわり

10.times do
    chars = html.scan(/">(.*?)<\/div><\/td>/).flatten.join("")
    p chars

    main.puts chars
    ans = nil
    num = nil
    Timeout::timeout(2) {
        ans,num = main.gets.chomp.split(" ")
    }
    count += num.to_i

    # post
    req = Net::HTTP::Post.new(uri.request_uri)
    req.set_form_data({'Seed' => seed, 'Started' => started, 'move' => ans, 'Moves' => moves}, ';')

    moves.push(ans)

    res = Net::HTTP.new(uri.host, uri.port).tap{|h| h.use_ssl=true}.start do |http|
        response = http.request(req)
        html = response.body
    end
end

p count
if count >= 2231 then

    HIURL = "https://icanhazwordz.appspot.com/highscores"
    hiuri = URI.parse(HIURL)

    req = Net::HTTP::Post.new(hiuri.path)
    req.set_form_data({'Seed' => seed, 'Started' => started, 'Agent' => 'Robot', 'Name' => 'Yuka Takahashi', 'Email' => 'yukatkh@gmail.com', 'NickName' => 'Yuka', 'URL' => 'https://github.com/yamaguchi1024', 'Moves' => moves}, ';')

    res = Net::HTTP.new(uri.host, uri.port).tap{|h| h.use_ssl=true}.start do |http|
        response = http.request(req)
        html = response.body
    end
end
