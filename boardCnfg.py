Import("env")

# print(env.Dump())

board_config = env.BoardConfig()
# should be array of VID:PID pairs
board_config.update("build.hwids", [
  [
    "0x5A68",
    "0x0005"
  ],
  [
    "0x1B4F",
    "0x9206"
  ],
  [
    "0x1B4F",
    "0x9205"
  ],
  [
    "0x2341",
    "0x8037"
  ],
  [
    "0x2341",
    "0x0037"
  ]
])