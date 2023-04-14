# player.py
# This is the game client that connects to the server.
# Format: python3 player.py PlayerName RoomName

from socket import *
import argparse
import sys
import signal
import selectors
from sys import stdin
from urllib.parse import urlparse

discoveryPort = 12001

try:
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
        clientSocket.sendto(message.encode(), ("localhost", int(serverPort)))

        # settimeout() relies on recvfrom being called, so we will read the data here
        # and then pass it to the receiveFromServer function
        data, addr = clientSocket.recvfrom(1000)
        if data:
            recieveFromServer(None, None, data)
        

    def recieveFromServer(conn, mask, datapass=None):
        # if we received data earlier, redirect it to data
        # otherwise read
        if datapass == None:
            data, addr = conn.recvfrom(1000)  # Should be ready
        else:
            data = datapass

        if data:
            # in case someone tries to connect with the same name, don't allow them to connect
            # this will be solved if i make each player have a unique ID instead of storing based on name
            if data.decode() == "This player already exists in this room" and not joined:
                print("Another player is already named '{}'. Pick another\n".format(player))
                sys.exit(0)

            # shut down the server for everyone
            if data.decode() == "shutdown":
                print("Skipping shutdown")
                #print("This room is shutting down, which means all your items are gone too. Thanks for playing!")
                #sys.exit(0)
            
            # move to the north room
            if data.decode().startswith("roomchange:"):
                changeRoom(data.decode().replace("roomchange:", ""))
            else:
                # otherwise, print the server data
                print(data.decode() + "\n")

    # function for changing rooms
    def changeRoom(data):
        # split into address/location and inventory
        
        # Foyer item1,item2,item3
        sep = data.split(" ")

        # look up room to see if it exists
        clientSocket.sendto("LOOKUP {}".format(sep[0]).encode(), ("localhost", discoveryPort))
        response, address = clientSocket.recvfrom(2048)
        response = response.decode()
        if response.startswith("NOTOK"):
            # bad
            print(response.replace("NOTOK ", ""))
            print("Failed to change rooms!")
            sys.exit(-1)
        else:
            # good
            add = response.replace("OK ", "")
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

    # parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("player_name")
    parser.add_argument("room_name")
    args = parser.parse_args()

    player = args.player_name
    serverRoom = args.room_name

    # create socket and selector
    sel = selectors.DefaultSelector()
    clientSocket = socket(AF_INET, SOCK_DGRAM)
    clientSocket.setblocking(False)

    clientSocket.settimeout(5)

    # try to connect to the server through discovery
    changeRoom(serverRoom)

    # register selectors
    sel.register(clientSocket, selectors.EVENT_READ, recieveFromServer)
    sel.register(stdin, selectors.EVENT_READ, getPlayerCommand)

    # join the server, get and print room information
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

# timed out after 5 secs, exit
except TimeoutError as e:
    print("Connection timed out. Exiting.")
    sys.exit(-1)