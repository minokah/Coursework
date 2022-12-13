## How to run
Start the discovery server first, then the server (which will automatically assign a port and send its port
and room name to the discovery server. Clients will then connect using the name of the room.

- Discovery: py discovery.py
- Server: py server.py Room "Description of Room" "item1" "item2" -n address
- Client: py player.py Brandon Room

### Commands
- look - Look around the room
- inventory - Check your inventory
- take (item) - Take an item off the ground
- drop (item) - Drop an item onto the ground
- say (text) - Talk to others within your room
- north/south/east/west - Move to different rooms NSEW
- exit - Exit the game