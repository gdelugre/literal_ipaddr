# Parsing IP addresses at compile-time in C++

This repository contains a C++17 constexpr implementation of the IP address parsing functions ``inet_addr``, ``inet_aton`` and ``inet_pton``. It supports both v4 and v6 address families.

When passed a literal IP address string, the compiler will parse the string and generate automatically the ``in_addr`` or ``in6_addr`` structure.

The address format is compatible with standard representations:
- ``inet_addr`` and ``inet_aton`` accept IPv4 addresses in format ``a.b.c.d``, ``a.b.c``, ``a.b``, or ``a``, with address components expressed in decimal, octal or hexadecimal.

```c++
// Alternative syntaxes producing the same result:
//   IPAddr::inet_addr("127.0.1")
//   IPAddr::inet_addr("127.1")
//   IPAddr::inet_addr("0x7f.1")
//   IPAddr::inet_addr("2130706433")
//
constexpr in_addr_t addr = IPAddr::inet_addr("127.0.0.1");
```

- ``inet_pton`` supports IPv4 addresses in canonical form (4 decimal components with no leading zeros) and IPv6 addresses represented according to RFC 2373, including with embedded IPv4 addresses.

```c++
constexpr auto in_addr1 = IPAddr::inet_pton<AF_INET>("127.0.0.1");
constexpr auto in_addr2 = IPAddr::inet_pton<AF_INET6>("::1");
constexpr auto in_addr3 = IPAddr::inet_pton<AF_INET6>("fe80::127.0.0.1"); // IPv4-in-IPv6 syntax
```

The ``ipaddr.h`` header also defines ``_ipaddr`` and ``_ipport`` literals. The ``_ipaddr`` suffix will automatically detect the format of the IP address and return the according ``in[6]_addr`` structure, or raise a static assertion if the format is incorrect.

## Example

```c++
int s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

const struct sockaddr_in dst {
    .sin_family = AF_INET,
    .sin_port   = 1234_ipport,
    .sin_addr   = "127.0.0.1"_ipaddr
};

connect(s, (struct sockaddr *)&dst, sizeof(dst));
```

The above example, with clang 8.0.0 on Linux x86_64, generates:

```objdump
        mov    $0x2,%edi
        mov    $0x1,%esi
        xor    %edx,%edx
        callq  <socket@plt>

        movups 0xe7e(%rip),%xmm0            # sockaddr in rodata section
        mov    %rsp,%rsi
        movaps %xmm0,(%rsi)

        mov    %eax,%edi
        mov    $0x10,%edx
        callq  <connect@plt>
```

With gcc 8.3.0:

```objdump
        xor    %edx,%edx
        mov    $0x1,%esi
        mov    $0x2,%edi
        callq  <socket@plt>

        mov    $0x10,%edx
        lea    0x8(%rsp),%rsi
        movabs $0x100007fd2040002,%rcx      # sockaddr as an immediate operand

        mov    %eax,%edi
        mov    %rcx,0x8(%rsp)
        movq   $0x0,0x10(%rsp)
        callq  <connect@plt>
```

## License

Distributed under the MIT license, see LICENSE file.
