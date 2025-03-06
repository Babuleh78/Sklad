def is_integer(s):
    try:
        x = int(s)
        return str(x) == s  
    except ValueError:
        return False

def is_float(s):
    try:
        x = float(s)
        return str(x) == s  
    except ValueError:
        return False

def is_nn_integer(s):
    try:
        x = int(s)
        return x >= 0  
    except ValueError:
        return False

a = input()
b = input()
c = input()

integer_value = None
float_value = None
nn_integer_value = None

for value in [a, b, c]:
    if is_nn_integer(value):
        nn_integer_value = value
    elif is_integer(value):
        integer_value = value
    elif is_float(value):
        float_value = value

if integer_value<nn_integer_value:
    nn_integer_value, integer_value = integer_value, nn_integer_value

print(integer_value, float_value, nn_integer_value)
