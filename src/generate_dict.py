import json
from collections import defaultdict

def generate_actor_dict():
    actor_movie_dict = defaultdict(list)
    
    json_list = json.loads(input())
    
    for movie in json_list:
        for actor in movie.get('cast', []):
            actor_movie_dict[actor].append(movie.get('title', 'Unknown Title'))
    
    with open('actor_movie_dict.json', 'w') as d:
        json.dump(actor_movie_dict, d, indent=4)

if __name__ == '__main__':
    generate_actor_dict()


