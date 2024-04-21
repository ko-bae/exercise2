import json
import argparse


def retrieve_list_of_movie() -> None:
    parser = argparse.ArgumentParser(
        description="Retrieve list of movies for a given actor"
    )
    parser.add_argument("actor_name", help="Actor's name")
    args = parser.parse_args()
    actor = args.actor_name

    with open("actor_movie_dict.json", "r") as d:
        actor_movie_dict = json.load(d)
    movies = "\n".join(actor_movie_dict.get(actor, []))

    print(movies)


if __name__ == "__main__":
    retrieve_list_of_movie()
