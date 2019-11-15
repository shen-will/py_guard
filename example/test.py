import game

print("test running")

def show_info():
    print ("show info")

    def hello():
        print ("hello info")
        pass

    hello()

    return hello


def show_game():
    game.main()