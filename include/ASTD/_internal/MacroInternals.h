// Copyright Alternity Arts. All Rights Reserved

#pragma once

#define STRINGIFY(x) #x
#define EXPAND(x) x
#define CONCAT(x, y) x##y
#define DOUBLE_CONCAT(x, y, z) CONCAT(CONCAT(x, y), EXPAND(z))