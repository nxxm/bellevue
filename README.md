# `belle::vue` : C++ HTML5 Views
A WebAssembly Browser templating engine for your HTML5 C++ application.

## Features

* HTML5 and Browser API integration : `belle::vue::get_element_by_id`, `Element::innerHTML`...
* HTML5 datamodel views : 

```html
<ul>
  <li v-for="products"> {{name}} : {{quantity}} </li>
</ul>
```

## Getting started
Clone it directly and play with the [examples/](./examples) or use the library in your app : 

* `#include <belle/vue.hxx>` with [`.nxxm/deps`](https://nxxm.github.io) : 

```
{
  "nxxm/bellevue" : {}
}
```

## License
This project is licensed under the permissive MIT License, this is done to encourage you to contribute and make the next generation of Web Apps in WebAssembly together. 

[MIT License](./LICENSE.md), please just reproduce copyrights : 

```
Copyright (c) 2018 - present nxxm.io
```

