# discovery.py
# This is the hub for the rooms. Servers will request a port and register it with this
# service, and then clients will connect to this service to get the room's address

from socket import *
from urllib.parse import urlparse

discoveryPort = 12001

# discovery service will always be on port 12001
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(("localhost", discoveryPort))

# map or dictionary to hold the addresses
# {Name, Address}
addressList = {}

print("Discovery service started on port", discoveryPort)

while (True):
    params, clientAddress = serverSocket.recvfrom(2048)
    params = params.decode()

    params = params.split(" ")

    command = params.pop(0)
    # register a new port with the room name
    if command == "REGISTER":
        if len(params) < 2:
            serverSocket.sendto("NOTOK Insufficient parameters".encode(), clientAddress)
            continue
    
        address = params[0]
        name = params[1]

        # register the room
        url = urlparse(address)
        port = url.netloc.split(":")[1]
        addressList[name] = "room://localhost:{}".format(port)
        serverSocket.sendto("OK".encode(), clientAddress)
        print("Registering {} on port {}".format(name, port))
    # free up port, deregister name
    elif command == "DEREGISTER":
        if len(params) < 1:
            serverSocket.sendto("NOTOK Please specify a room name".encode(), clientAddress)
            continue

        name = params[0]

        if name not in addressList:
            serverSocket.sendto("NOTOK {} is not a registered room".format(name).encode(), clientAddress)
        else:
            # remove the port
            print("Deregistered {}, port {}".format(name, addressList[name]))
            addressList.pop(name)
    # look up a room name to see if it exists
    elif command == "LOOKUP":
        if len(params) < 1:
            serverSocket.sendto("NOTOK A name must be specified".encode(), clientAddress)
            continue
        
        name = params[0]

        if name not in addressList:
            serverSocket.sendto("NOTOK {} was not found in registered rooms".format(name).encode(), clientAddress)
        else:
            serverSocket.sendto("OK {}".format(addressList[name]).encode(), clientAddress)
            print("{} successfully requested lookup for {}".format(clientAddress, name))
    else:
        serverSocket.sendto("NOTOK Invalid command".encode(), clientAddress)

