// Copyright Alternity Arts. All Rights Reserved

#pragma once

#define STRINGIFY(x) #x
#define EXPAND(x) x

#define CONCAT(x, y) x##y
#define CONCAT_EXPAND(x, y) CONCAT(x,y)

#define DOUBLE_CONCAT(x, y, z) CONCAT_EXPAND(CONCAT_EXPAND(x, y), z)