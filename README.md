# My ESPHome configs and components

This repository contains my personal [ESPHome](https://esphome.io/index.html) configurations and custom components.

## External Components

I have implemented the following custom components:

- [Fnordlicht Light Output](./components/fnordlicht)
- [Xiaomi Antibacterial Air Humidifier](./components/xiaomi_deerma_humidifier/)

In order to use these components in your ESPHome configs, you will need to add the following snippet to your device configs:

```yaml
external_components:
  source:
    type: git
    url: https://github.com/stv0g/esphome-config
```

## Author

- Steffen Vogel ([@stv0g](https://github.com/stv0g))

## License

Copyright 2023 Steffen Vogel

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
