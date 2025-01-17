def davinci():
    input_data = input()
    paintings = input_data.split(", ")
    affordable_paintings = []
    for painting in paintings:
        name, price = painting.split(" - ")
        price = int(price)
        if price % 500 == 0:
            affordable_paintings.append(name)
    affordable_paintings.sort()
    print(", ".join(affordable_paintings))
davinci()