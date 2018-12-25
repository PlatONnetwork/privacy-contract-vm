#!/bin/bash

slice2cpp --underscore -I $ICE_ROOT/slice *.ice
slice2java --underscore -I $ICE_ROOT/slice *.ice

