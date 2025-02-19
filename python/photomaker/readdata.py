import json

def get_data():
    with open('data.json', 'r') as f:
        loaded_data = json.load(f)

    return loaded_data

