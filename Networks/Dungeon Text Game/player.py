# player.py
# This is the game client that connects to the server.
# Format: python3 client.py PlayerName room://address:port

from socket import *
import argparse
import sys
import signal
import selectors
from sys import stdin
from urllib.parse import urlparse

# client variables
player = None
joined = False
clientSocket = None
serverName = None
serverPort = None

# get input from stdin
def getPlayerCommand(conn, mask):
    message = input("")

    # exit
    if (message == "exit"):
        terminate(None, None)

    communicate("{}:{}".format(player, message))

# send, parse and print server message responses
def communicate(message):
    clientSocket.sendto(message.encode(), (serverName, int(serverPort)))

def recieveFromServer(conn, mask):
    data, addr = conn.recvfrom(1000)  # Should be ready

    if data:
        # in case someone tries to connect with the same name, don't allow them to connect
        # this will be solved if i make each player have a unique ID instead of storing based on name
        if data.decode() == "This player already exists in this room" and not joined:
            print("Another player is already named '{}'. Pick another\n".format(player))
            sys.exit(0)

        # shut down the server for everyone
        if data.decode() == "shutdown":
            print("This room is shutting down, which means all your items are gone too. Thanks for playing!")
            sys.exit(0)
        
        # move to the north room
        if data.decode().startswith("north:"):
            changeRoom(data.decode(), "north")
        elif data.decode().startswith("south:"):
            changeRoom(data.decode(), "south")
        elif data.decode().startswith("east:"):
            changeRoom(data.decode(), "east")
        elif data.decode().startswith("west:"):
            changeRoom(data.decode(), "west")
        else:
            # otherwise, print the server data
            print(data.decode() + "\n")
    else:
        print('Terminating connection', conn)
        sel.unregister(conn)
        conn.close()

def changeRoom(str, dir):
    # split into address/location and inventory
    sep = str.split(" ")

    # split the address
    add = sep[0].strip("{}:".format(dir))

    newurl = urlparse(add)

    newloc = newurl.netloc.split(":")
    global serverName
    global serverPort
    serverName = newloc[0]
    serverPort = newloc[1]

    # if inventory
    if len(sep) > 1:
        communicate("{}:join {}".format(player, sep[1]))
    else:
        # join like a new user
        communicate("{}:join".format(player))

# disconnect from server and quit
def terminate(sig, frame):
    print("Dropping from server... thanks for playing!")
    communicate("{}:exit".format(player))
    sys.exit(0)

try:
    # parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("player_name")
    parser.add_argument("server_address")
    args = parser.parse_args()

    player = args.player_name
    server = args.server_address

    # attempt connection to server
    url = urlparse(server)
    if url.scheme != "room":
        raise Exception("Invalid address. Format: room://address:port")

    urlloc = url.netloc.split(":")
    if (len(urlloc) < 2):
        raise Exception("Invalid address. Format: room://address:port")
    serverName = urlloc[0]
    serverPort = urlloc[1]

    if not serverPort.isdigit():
        raise Exception("Invalid address. Format: room://address:port")

    # create socket and selector
    sel = selectors.DefaultSelector()
    clientSocket = socket(AF_INET, SOCK_DGRAM)
    clientSocket.setblocking(False)
    sel.register(clientSocket, selectors.EVENT_READ, recieveFromServer)
    sel.register(stdin, selectors.EVENT_READ, getPlayerCommand)

    # join the server, get and print room information
    communicate("{}:join".format(player))
    joined = True # join successful

    # allow termination
    signal.signal(signal.SIGINT, terminate)

    # let user enter commands now
    while True:
        events = sel.select()
        for key, mask in events:
            callback = key.data
            callback(key.fileobj, mask)

    clientSocket.close()
except Exception as e:
    print(e)
    print("Failed to connect to the server! Check your address")
    
    # just in case, send a terminate client message to the server
    terminate(None, None)