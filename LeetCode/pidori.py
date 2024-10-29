import numpy as np
def f(p):
    return p * (1 + np.exp(p**2)) / (1 - np.exp(p**2))
def runge_kutta(h, y0, p0, x0, x_end):
    x_values = []
    y_values = []
    p_values = []
    x = x0
    y = y0
    p = p0
    while x <= x_end:
        x_values.append(x)
        y_values.append(y)
        p_values.append(p)
        k1 = h * p
        l1 = h * f(p)
        k2 = h * (p + l1 / 2)
        l2 = h * f(p + l1 / 2)
        k3 = h * (p + l2 / 2)
        l3 = h * f(p + l2 / 2)
        k4 = h * (p + l3)
        l4 = h * f(p + l3)
        y += (k1 + 2*k2 + 2*k3 + k4) / 6
        p += (l1 + 2*l2 + 2*l3 + l4) / 6
        x += h
    return x_values, y_values, p_values
# Начальные условия
y0 = 3
p0 = 0 # Начальное значение производной
x0 = 1
x_end = 5
h = 0.001 # Шаг интегрирования
x_values, y_values, p_values = runge_kutta(h, y0, p0, x0, x_end)
# Вывод результатов
for x, y in zip(x_values, y_values):
    print(f"x: {x:.2f}, y: {y:.4f}")