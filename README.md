The purpose of this project is to build a chromium service in chromium codebase, and a windows local background service. The chromium service, whenever gets launched, starts the windows background service through the service control manager handler.
The windows service opens the windows notepad whenever it gets started by the chromium service.
I have added the files of the chromium service and the document where the code of the avatar toolbar button is displayed. This function gets called whenever the profile icon gets clicked.
