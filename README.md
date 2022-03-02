# SH:ell E:mpties R:ecycle B:in (sherb)
`sherb` was designed with one goal in mind; to provide a quick and simple way to clean out your system's recycle bin via the terminal. The application was developed using [C](#) and currently only supports running on the Windows operating system. Due to stripping out the use of [C](#) runtime libraries and having such a small code footprint, our application comes out at a very tiny **6kb** of size.

## Usage
Below are examples of how to use all commands inside of `sherb`...

* `sherb` – Clears the entire recycle bin. Outputs a beep once finished (if you have system sounds on).
* `sherb -q` – Silently clears the entire recycle bin.
* `sherb -d a,b,...` – Clears the recycle bin of files from the specified drives.
* `sherb -h` – Prints a some extended help information in order to assist the user.
* `sherb -V` – Prints the current sherb version to the terminal.
* `sherb -L` – Prints a block of license text to the terminal.

It should be noted that the commands `-h,-V,-L` are all blocking and will prevent the usage of `-q,-d`.

## Install
[Download]() the binary and put it anywhere on your system that is connected to your [`%PATH%`](https://stackoverflow.com/a/28778358/64949).

## Building from source
*Coming soon...*

## License
`sherb` comes with two licenses (MIT and Unlicense) of which you can choose the one you prefer. For more info, see: [LICENSE](LICENSE)

## Notes
This project would not exist without originally running into the [empty-recycle-bin](https://github.com/sindresorhus/empty-recycle-bin) while browsing the [Chocolatey](https://chocolatey.org/) community package repository. Our reasons for development were more than likely completely different, and thus `sherb` was produced.
