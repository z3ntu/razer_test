#!/usr/bin/env python3

import json
import glob
from collections import OrderedDict

# The order the keys should be in
KEYORDER = [
    "name",
    "vid",
    "pid",
    "type",
    "pclass",
    "leds",
    "fx",
    "features",
    "quirks",
    "matrix_dimensions",
    "max_dpi"
]


class MyJSONEncoder(json.JSONEncoder):
    """A JSON Encoder that formats the JSON 'correctly'."""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.indentation_level = 0
        self.indent = 4

    def encode(self, o):
        """Encode JSON object *o*."""
        if isinstance(o, (list, tuple)):
            # If the children are just str and int, keep on a single line
            if any(isinstance(el, (str, int)) for el in o):
                return "[" + ", ".join(json.dumps(el) for el in o) + "]"
            else:
                self.indentation_level += 1
                output = [self.indent_str + self.encode(el) for el in o]
                self.indentation_level -= 1
                return "[\n" + ",\n".join(output) + "\n" + self.indent_str + "]"

        elif isinstance(o, dict):
            self.indentation_level += 1
            output = [self.indent_str +
                      f"{json.dumps(k)}: {self.encode(v)}" for k, v in o.items()]
            self.indentation_level -= 1
            return "{\n" + ",\n".join(output) + "\n" + self.indent_str + "}"

        else:
            return json.dumps(o)

    @property
    def indent_str(self) -> str:
        return " " * self.indentation_level * self.indent


# Iterate through all json files
for jsonfile in glob.glob('./data/devices/*.json'):
    with open(jsonfile, 'r') as f:
        # Read file
        data = json.load(f, object_pairs_hook=OrderedDict)
        # Sort the device entries by PID
        data.sort(key=lambda device: device["pid"])
        # Iterate through all devices in the file
        for device in data:
            # Order keys
            for key in KEYORDER:
                try:
                    device.move_to_end(key)
                except KeyError:
                    #print("Didn't find " + key)
                    pass
            # Remove empty keys
            for key in KEYORDER:
                value = device.get(key)
                if isinstance(value, list) and len(value) == 0:
                    device.pop(key)
            # Remove default matrix_dimensions key [0, 0]
            value = device.get("matrix_dimensions")
            if value is not None and len(value) > 1 and value[0] == 0 and value[1] == 0:
                device.pop("matrix_dimensions")
            # Sort leds array
            value = device.get("leds")
            if value is not None:
                value.sort()
            # TODO Find sensible sorting for quirks, fx & features
    with open(jsonfile, 'w') as f:
        # Write back to file
        # json.dump() isn't used because it calls iterencode() instead of encode() internally
        jsonstr = json.dumps(data, cls=MyJSONEncoder)
        f.write(jsonstr)
