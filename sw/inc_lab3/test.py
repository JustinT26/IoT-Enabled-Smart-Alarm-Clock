
import math
import numpy 


array_sin = [0] * 60
array_cos = [0] * 60

for i in range(0, 360, 6):
    rad = math.radians(i)   # convert degrees → radians
    array_sin.append(round(math.sin(rad) * 1024))
    array_cos.append(round(math.cos(rad) * 1024))
    
print("sin array:\n")
print(array_sin)
print("\ncos array:\n")
print(array_cos)