cocos2dx-autoBatchNode
======================

Automatic batching implementation for isometry scenes for cocos2dx. You dont have to 
wait for cocos2dx 3.0!
IsometryNode is a node like CCBatchNode, but supports any number of textures on it. 

Some issues:
- can be used only with IsometrySprite whitch is CCSprite descendant
- dosen't support object rotation.
- uses some hacks to avoid changing of cocos2dx source - tested with 2.1.4
