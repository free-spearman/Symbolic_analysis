import Driver
"""PARAM_LABEL = '#'
src_name = "SymIm_0.080000_0.080000_100"
data_file = open(src_name,'r')
for line in data_file:
    if (line[0] != PARAM_LABEL):
        continue
    print(line[1:])
"""
print("begin")
test = Driver.DriverMatplotlib("SymIm_0.080000_0.080000_100")
test.parse_data_file()
test.draw_sym_im()
print("end")