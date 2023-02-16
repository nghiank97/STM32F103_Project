
import numpy as np
import math
import sys

with open('E:\Stm32F103\ENC28J60\html\LED\html\sort\data.txt','r') as f:
    lines = f.read()

list_of_values = lines.split(' ')
len_data = len(list_of_values)

header = "HTTP/1.1 200 OK\r\nContent-Type: ".encode('utf-8')
h_a = [hex(t) for t in header]

len_data += len(header)

with open('E:\Stm32F103\ENC28J60\html\LED\html\sort\\result.txt', 'w') as f:

    f.write('#include "html.h"\n'.format(len_data))
    f.write("extern const u08 html_main[{}] = {} \n".format(len_data, '{'))

    for i in header:
        f.write(str(i) + ',')
    f.write('\n')

    for i,line in enumerate(list_of_values):
        if i%32 == 0 and i>0:
            f.write('\n')
        f.write(str(line) + ',')

    f.write("\n{};\n".format('}'))