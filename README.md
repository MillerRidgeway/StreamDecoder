# StreamDecoder
Framing stream decoder written in C++

# Usage
On a Linux system:

  ```cd bin```
  
  ```./StreamDecoder```

On any other system (using CMake):

  ```cd bin```
  
  ```ccmake ..```

Step through the CMake config menu using *C* followed by *G*. After that you the *bin* will be populated, so you can call 

  ```./StreamDecoder```

The default behavior is to run through a handful of tests cases written in the file ```main.cpp```. If you wish to add more test cases, please do so using the following steps:

1. Open the ```main.cpp``` file in an editor of your choice.
2. Create an array of type ```uint8_t``` containg the stream you would like to test.
3. Push the array you just created onto testing vectors using the following code:

  ```packets.push_back(name_of_arr);```
  
  ```sizes.push_back(sizeof(name_of_arr));```
  
4. After you have created your new test case, make sure you re-build the file with ```make```. To do so
   ```cd bin```
  
   ```make```
  
   ```./StreamDecoder```
