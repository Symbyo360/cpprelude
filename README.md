# CPPrelude

**CPPrelude** is a body of code that I tend to write over and over again in every project I make. So, I decided to write it as a library and include it in my projects.

It contains algorithms, containers, memory management stuff ... etc.

Developed by:

**[Moustapha Saad Abdel-hameed](mailto:moustapha.saad.abdelhamed@gmail.com)**

**[Nora Abdel-hameed](mailto:nora.abdelhameed@gmail.com)**

**CPPrelude** version 5.0.0 is released under the GNU Lesser General Public License version 3.

Check out the [examples](scratch/src/examples.cpp).

## Philosophy and Coding standard

For Philosophy and coding standard please refer to this [link](https://moustaphasaad.github.io/2017/08/05/cpprelude/).

[How CPPrelude does polymorphism?](https://moustaphasaad.github.io/2018/01/06/How-CPPrelude-does-polymorphism/)

## Building

1. **CPPrelude** uses premake5 as its meta build system so you have to download premake5
2. Use premake5 to generate project/solution files in order to compile the library.
   1. Ex: `premake5 gmake` this will generate the build folder with linux make files inside
   2. Ex: `premake5 vs2015` this will generate the build folder with visual studio 2015 solution files inside
3. **CPPrelude** supports 32-bit and 64-bit targets and has a *debugShared* and *releaseShared* modes
4. building the library is as easy as invoking your build system
5. binaries should be generated inside a bin folder right beside the build folder
6. Also a unittest executable will be generated you can invoke it to test your **CPPrelude** build

## Content

- **[algorithm](docs/Files/algorithm.md):** a collection algorithms that could be used with the provided containers.
- **[allocator](docs/Files/allocator.md):** allocators that could be used with the provided containers.
- **[array](docs/Files/array.md):** a fixed size array.
- **[bucket_array](docs/Files/bucket_array.md):** a bucket array container.
- **[bufio](docs/Files/bufio.md):** a buffered input/output.
- **[defines](docs/Files/defines.md):** languages primitives.
- **[dlinked_list](docs/Files/dlinked_list.md):** a double linked list.
- **[dynamic_array](docs/Files/dynamic_array.md):** a dynamic grow-able array.
- **[error](docs/Files/error.md):** a collection of error reporting functions.
- **[file](docs/Files/file.md):** a file stream.
- **[file_defs](docs/Files/file_defs.md):** OS specific file handles
- **[fmt](docs/Files/fmt.md):** a collection standard print/scan functions
- **[hash_array](docs/Files/hash_array.md):** a hash array implementation.
- **[io](docs/Files/io.md):** a basic stream input/output implementation.
- **[memory](docs/Files/memory.md):** a basic memory slice primitive.
- **[memory_context](docs/Files/memory_context.md):** a memory context/allocator trait.
- **[memory_watcher](docs/Files/memory_watcher.md):** a memory leak scope watcher.
- **[platform](docs/Files/platform.md):** an abstraction on the actual OS.
- **[priority_queue](docs/Files/priority_queue.md):** a heap implementation.
- **[queue_array](docs/Files/queue_array.md):** a queue implementation based on a dynamic_array data structure.
- **[queue_list](docs/Files/queue_list.md):** a queue implementation based on a dlinked_list data structure.
- **[result](docs/Files/result.md):** a result the combines a value and an error into the same structure in a transparent manner.
- **[slinked_list](docs/Files/slinked_list.md):** a single linked list implementation.
- **[stack_array](docs/Files/stack_array.md):** a stack implementation based on a dynamic_array data structure.
- **[stack_list](docs/Files/stack_list.md):** a stack implementation based on a slinked_list data structure.
- **[stream](docs/Files/stream.md):** a memory stream implementation.
- **[string](docs/Files/string.md):** an UTF-8 string implementation.
- **[tree_map](docs/Files/tree_map.md):** a red black tree implementation.

## How to contribute

Feel free to fork and create pull requests at any time. It would be nice if you involved any one of us with you from the start to guide you, or maybe discuss your idea itself (Developers e-mails are up above).

## Thanks
- [Ahmed Hesham](mailto:ahesham@outlook.com) for adding Visual Studio debug visualizer.