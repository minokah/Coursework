# server.py
# This is a server for a room. It can hold items on the floor.
# Format: python3 server.py RoomName "Room Description" "item1" "item2" "itemN" [-n address]
# Add on -n name, or -s name etc etc to link up rooms

from socket import *
import sys
import signal
import argparse
from urllib.parse import urlparse

# discovery port
discoveryPort = 12001

serverSocket = socket(AF_INET, SOCK_DGRAM)

# terminate server
def terminate(sig, frame):
    print("Shutting down server...")

    # deregister, then send shutdown to clients
    serverSocket.sendto("DEREGISTER {}".format(args.name).encode(), ("localhost", discoveryPort))

    for names in players:
        serverSocket.sendto(("shutdown").encode(), players[names]["address"])
    sys.exit(0)

# argument parsing for room setup
parser = argparse.ArgumentParser()
parser.add_argument("name")
parser.add_argument("desc")
parser.add_argument("items", nargs="*")
parser.add_argument("-n")
parser.add_argument("-s")
parser.add_argument("-e")
parser.add_argument("-w")
args = parser.parse_args()

print("Attempting to allocate a port...")

# find if room already exists
serverSocket.sendto("LOOKUP {}".format(args.name).encode(), ("localhost", discoveryPort))
roomCheck, add = serverSocket.recvfrom(2048)
roomCheck = roomCheck.decode()

# room name already exists/lookup failed, exit
if roomCheck.startswith("OK"):
    print(args.name, "was already found in the room list. Pick another room name")
    sys.exit(-1)

# otherwise, continue
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(("localhost", 0))
serverPort = serverSocket.getsockname()[1]
print("This room's port is", serverPort)

serverSocket.sendto("REGISTER room://localhost:{} {}".format(serverPort, args.name).encode(), ("localhost", discoveryPort))
registerResponse, add = serverSocket.recvfrom(2048)
registerResponse = registerResponse.decode()

# final check for OK
if not registerResponse.startswith("OK"):
    print(registerResponse.replace("NOTOK ", ""))
    sys.exit(-1)

# check if the adjacent room URLs are valid, skip if not
north = None
south = None
east = None
west = None

# lookup the room, if it is registered return true
def findRoom(name):
    serverSocket.sendto("LOOKUP {}".format(name).encode(), ("localhost", discoveryPort))
    address, add = serverSocket.recvfrom(2048)
    address = address.decode()

    if address.startswith("NOTOK"):
        return False
    
    return True

# assign room names from params
if args.n:
    north = args.n
    print("North:", north)
if args.s:
    south = args.s
    print("South:", south)
if args.e:
    east = args.e
    print("East", east)
if args.w:
    west = args.w
    print("West:", west)

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
    
    # move north, south, east, or west
    elif command == "north":
        if not north or not findRoom(north):
            serverSocket.sendto("There is no northern room.".encode(), players[user]["address"])
        else:
            for name in players:
                if name != user:
                    serverSocket.sendto("{} is moving to the northern room.".format(user).encode(), players[name]["address"])
            serverSocket.sendto("Moving to the northern room.".encode(), players[user]["address"])
            serverSocket.sendto("-----------------------".encode(), players[user]["address"])
            serverSocket.sendto("roomchange:{} {}".format(north, ",".join(players[user]["items"])).encode(), players[user]["address"])
            print("{} is moving to the northern room".format(user))

            players.pop(user)
    elif command == "south":
        if not south or not findRoom(south):
            serverSocket.sendto("There is no southern room.".encode(), players[user]["address"])
        else:
            for name in players:
                if name != user:
                    serverSocket.sendto("{} is moving to the southern room.".format(user).encode(), players[name]["address"])
            serverSocket.sendto("Moving to the southern room.".encode(), players[user]["address"])
            serverSocket.sendto("-----------------------".encode(), players[user]["address"])
            serverSocket.sendto("roomchange:{} {}".format(south, ",".join(players[user]["items"])).encode(), players[user]["address"])
            print("{} is moving to the southern room".format(user))

            players.pop(user)
    elif command == "east":
        if not east or not findRoom(east):
            serverSocket.sendto("There is no eastern room.".encode(), players[user]["address"])
        else:
            for name in players:
                if name != user:
                    serverSocket.sendto("{} is moving to the eastern room.".format(user).encode(), players[name]["address"])
            serverSocket.sendto("Moving to the eastern room.".encode(), players[user]["address"])
            serverSocket.sendto("-----------------------".encode(), players[user]["address"])
            serverSocket.sendto("roomchange:{} {}".format(east, ",".join(players[user]["items"])).encode(), players[user]["address"])
            print("{} is moving to the eastern room".format(user))

            players.pop(user)
    elif command == "west":
        if not west or not findRoom(west):
            serverSocket.sendto("There is no western room.".encode(), players[user]["address"])
        else:
            for name in players:
                if name != user:
                    serverSocket.sendto("{} is moving to the western room.".format(user).encode(), players[name]["address"])
            serverSocket.sendto("Moving to the western room.".encode(), players[user]["address"])
            serverSocket.sendto("-----------------------".encode(), players[user]["address"])
            serverSocket.sendto("roomchange:{} {}".format(west, ",".join(players[user]["items"])).encode(), players[user]["address"])
            print("{} is moving to the western room".format(user))

            players.pop(user)

    # drop all items from client
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