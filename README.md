# zhavam (C project)
Front end for music recognition over Linux

Zhavam connects with Alsa or Pulse Drivers to record a small part of the song and then sends it to ACRCloud. ACRCloud responds with the song data in a Json message. The data recovered from the service includes the title of the song, the artist, the album, the genre of the year, etc.
zhavam analyzes this message and presents the most relevant information in the Track information text view.

For using zhavam in your computer you have to have an account in ACRCloud. Registering in ACRCloud brings you the host to access through the access key and access secret required to start a session with teh server.

Before using zhavam, please register yourself in ACRCloud and put the host, access key and access secret into the config file of zhavam.
