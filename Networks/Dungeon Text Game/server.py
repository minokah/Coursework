# server.py
# This is a server for a room. It can hold items on the floor.
# Format: python3 server.py port RoomName "Room Description" "item1" "item2" "itemN" [-n address]
# Add on -n address, or -s address etc etc to link up rooms

from socket import *
import sys
import signal
import argparse
from urllib.parse import urlparse

# terminate server
def terminate(sig, frame):
    print("Shutting down server...")
    for names in players:
        serverSocket.sendto(("shutdown").encode(), players[names]["address"])
    sys.exit(0)

# argument parsing for room setup
parser = argparse.ArgumentParser()
parser.add_argument("port")
parser.add_argument("name")
parser.add_argument("desc")
parser.add_argument("items", nargs="*")
parser.add_argument("-n")
parser.add_argument("-s")
parser.add_argument("-e")
parser.add_argument("-w")
args = parser.parse_args()

print("Creating server...")

# create the server
serverPort = args.port
if not serverPort.isdigit():
    print("Invalid port number, it must be an integer")
    sys.exit(-1)

# check if rooms are valid
def checkRoomURL(url):
    # attempt connection to server
    url = urlparse(url)
    if url.scheme != "room":
        return False

    urlloc = url.netloc.split(":")
    if (len(urlloc) < 2):
        return False

    if not serverPort.isdigit():
        return False
    
    return True

# check if the adjacent room URLs are valid
north = None
south = None
east = None
west = None

try:
    if args.n:
        if not checkRoomURL(args.n): 
            raise Exception("Invalid address for North room. Format: room://address:port")
        else: 
            north = args.n
            print("North:", north)
    if args.s:
        if not checkRoomURL(args.s): 
            raise Exception("Invalid address for South room. Format: room://address:port")
        else: 
            south = args.s
            print("South:", south)
    if args.e:
        if not checkRoomURL(args.e): 
            raise Exception("Invalid address for East room. Format: room://address:port")
        else: 
            east = args.e
            print("East", east)
    if args.w:
        if not checkRoomURL(args.w): 
            raise Exception("Invalid address for West room. Format: room://address:port")
        else: 
            west = args.w
            print("West:", west)
except Exception as e:
    print(e)
    sys.exit(-1)

serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(("localhost", int(serverPort)))

print("This room's port is", serverPort)

# server stuff
allowed_commands = [
    "look - Look around the room",
    "inventory - Check your inventory",
    "take (item) - Take an item off the ground", 
    "drop (item) - Drop an item onto the ground", 
    "say (text) - Talk to others within your room",
    "north - If possible, go to the northern room",
    "south - If possible, go to the southern room",
    "east - If possible, go to the eastern room",
    "west - If possible, go to the western room",
    "exit - Leave the game"
]

# list of clients and their stats/inventories
players = {}

# room settings
floor_items = []

# add items to floor
for i in range(0, len(args.items)):
    floor_items.append(args.items[i])

# generate the default room message
def generateRoomMessage():
    message = "{}\n{}\n\n".format(args.name, args.desc)
    # subtract 1 to account for individual player
    if len(floor_items) == 0 and len(players.keys())  - 1 == 0:
        message += "There is nothing in this room."
    else:
        message += "In this room, there are:\n"
        for i in range(0, len(floor_items)):
            message += floor_items[i] + "\n"
        for i in range(0, len(players.keys())):
            message += list(players.keys())[i] + "\n"
    return message

print("Server ready")
print("-----------------------")
print(generateRoomMessage())
print("-----------------------")


# allow termination
signal.signal(signal.SIGINT, terminate)

# await client...
while True:
    message, clientAddress = serverSocket.recvfrom(2048)
    # print(clientAddress)
    message = message.decode()
    print("Received: {} ({})".format(message, clientAddress))

    # user:command arg1 arg2 arg3
    message = message.split(" ")
    pre = message.pop(0)

    # arg1 arg2 arg3
    params = " ".join(message)

    # user:command
    pre = pre.split(":")
    user = pre[0]
    command = pre[1]

    if command == "join": # incoming client
        # player already exists
        if user in players:
            print("User {} already exists, dropping address ({} on port {})".format(user, clientAddress, serverPort))
            serverSocket.sendto("This player already exists in this room".format(user).encode(), clientAddress)
            continue

        # alert players of new join
        for name in players:
            serverSocket.sendto("{} has entered the {}".format(user, args.name).encode(), players[name]["address"])

        # otherwise if they also have incoming inventory
        if len(params) > 0:
            players[user] = {
            "address": clientAddress,
            "items": params.split(","),
        }
        else:
            # if no inventory, create new user inventory
            players[user] = {
                "address": clientAddress,
                "items": [],
            }

        print("User {} joined from address ({} on port {})".format(user, clientAddress, serverPort))

        serverSocket.sendto(generateRoomMessage().replace(user + "\n", "").encode(), clientAddress)
    elif command == "look":
        serverSocket.sendto(generateRoomMessage().replace(user + "\n", "").encode(), clientAddress)
    elif command == "take":
        if params in players:
            serverSocket.sendto("You cannot pick up another player".encode(), clientAddress)
        elif params in floor_items:
            players[user]["items"].append(params)
            floor_items.remove(params)
            print("{} took {}".format(user, params))
            serverSocket.sendto(("You took the {}".format(params)).encode(), clientAddress)
        else:
            serverSocket.sendto(("{} doesn't exist in this room".format(params)).encode(), clientAddress)
    elif command == "drop":
        if params in players[user]["items"]:
            players[user]["items"].remove(params)
            floor_items.append(params)
            print("{} dropped {}".format(user, params))
            serverSocket.sendto(("You dropped the {}".format(params)).encode(), clientAddress)
        else:
            serverSocket.sendto(("{} doesn't exist in your inventory".format(params)).encode(), clientAddress)
    elif command == "inventory":
        message = ""
        if len(players[user]["items"]) > 0:
            message = "Your inventory consists of:\n"
            for i in range (0, len(players[user]["items"])):
                message += players[user]["items"][i] + "\n"
        else:
            message = "Your inventory consists of nothing."

        serverSocket.sendto(message.encode(), clientAddress)

    elif command == "say":
        if len(params) > 0:
            for name in players:
                who = user
                
                # shows yourself as Name (You)
                if name == user:
                    who = "{} (You)".format(user)
                    
                serverSocket.sendto("{}: {}".format(who, params).encode(), players[name]["address"])
        else:
            serverSocket.sendto("You must enter enter some text".encode(), players[user]["address"])
    elif command == "north":
        if not north:
            serverSocket.sendto("There is no northern room.".encode(), players[user]["address"])
        else:
            for name in players:
                if name != user:
                    serverSocket.sendto("{} is moving to the northern room.".format(user).encode(), players[name]["address"])
            serverSocket.sendto("Moving to the northern room.".encode(), players[user]["address"])
            serverSocket.sendto("-----------------------".encode(), players[user]["address"])
            serverSocket.sendto("north:{} {}".format(north, ",".join(players[user]["items"])).encode(), players[user]["address"])
            print("{} is moving to the northern room".format(user))

            players.pop(user)
    elif command == "south":
        if not south:
            serverSocket.sendto("There is no southern room.".encode(), players[user]["address"])
        else:
            for name in players:
                if name != user:
                    serverSocket.sendto("{} is moving to the southern room.".format(user).encode(), players[name]["address"])
            serverSocket.sendto("Moving to the southern room.".encode(), players[user]["address"])
            serverSocket.sendto("-----------------------".encode(), players[user]["address"])
            serverSocket.sendto("south:{} {}".format(south, ",".join(players[user]["items"])).encode(), players[user]["address"])
            print("{} is moving to the southern room".format(user))

            players.pop(user)
    elif command == "east":
        if not east:
            serverSocket.sendto("There is no eastern room.".encode(), players[user]["address"])
        else:
            for name in players:
                if name != user:
                    serverSocket.sendto("{} is moving to the eastern room.".format(user).encode(), players[name]["address"])
            serverSocket.sendto("Moving to the eastern room.".encode(), players[user]["address"])
            serverSocket.sendto("-----------------------".encode(), players[user]["address"])
            serverSocket.sendto("east:{} {}".format(east, ",".join(players[user]["items"])).encode(), players[user]["address"])
            print("{} is moving to the eastern room".format(user))

            players.pop(user)
    elif command == "west":
        if not west:
            serverSocket.sendto("There is no western room.".encode(), players[user]["address"])
        else:
            for name in players:
                if name != user:
                    serverSocket.sendto("{} is moving to the western room.".format(user).encode(), players[name]["address"])
            serverSocket.sendto("Moving to the western room.".encode(), players[user]["address"])
            serverSocket.sendto("-----------------------".encode(), players[user]["address"])
            serverSocket.sendto("west:{} {}".format(west, ",".join(players[user]["items"])).encode(), players[user]["address"])
            print("{} is moving to the western room".format(user))

            players.pop(user)
    elif command == "exit":
        items_dropped = None
        
        # player crashed and doesn't exist on server
        if user not in players:
            print("Failed connection: player {} from {}".format(user, clientAddress))
            continue

        if len(players[user]["items"]) == 0:
            items_dropped = " nothing"
        else:
            items_dropped = ": " + ", ".join(players[user]["items"])

        for i in range(0, len(players[user]["items"])):
            floor_items.append(players[user]["items"][i])

        players.pop(user)
        print("Dropping {} from server. They dropped{}".format(user, items_dropped))
        
        # alert players of new join
        for name in players:
            serverSocket.sendto("{} has disconnected from the game".format(user).encode(), players[name]["address"])

        serverSocket.sendto("You dropped{}\n\nThanks for playing!".format(items_dropped).encode(), clientAddress)
    elif command == "help":
        serverSocket.sendto(("What can I do...?:\n" + "\n".join(allowed_commands)).encode(), clientAddress)
    else:
        serverSocket.sendto("Invalid command".encode(), clientAddress)