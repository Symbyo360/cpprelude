# File `io.h`

## Struct `io_trait`
Represents the input/output trait.

### Function `write`
```C++
usize
write(const slice<byte>& data);
```
Writes the given data to the trait.

1. **data**: data to be written to the trait.

- **Returns:** the actual amount written to the trait.

```C++
usize written_size = trait->write(data);
```


### Function `read`
```C++
usize
read(slice<byte>& data);

usize
read(slice<byte>&& data);
```
Reads into the given data from the trait. limit itself to the size of the given slice.

1. **data**: slice of data to read into.

- **Returns:** the actual amount read from the trait.

```C++
usize read_size = trait->read(data);
```


## Variable `cppr_out`
```C++
API_CPPR extern io_trait* cppr_stdout;
```
The standard output trait.


## Variable `cppr_err`
```C++
API_CPPR extern io_trait* cppr_stderr;
```
The standard error trait.


## Variable `cppr_in`
```C++
API_CPPR extern io_trait* cppr_stdin;
```
The standard input trait.