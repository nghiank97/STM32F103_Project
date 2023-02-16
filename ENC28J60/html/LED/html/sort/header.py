
header = "HTTP/1.1 200 OK\r\nContent-Type: ".encode('utf-8')
h_a = ["0x{:02X}".format(t) for t in header]
print(h_a)