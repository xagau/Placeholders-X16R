// Copyright (c) 2014 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PLACEH_ZMQ_ZMQCONFIG_H
#define PLACEH_ZMQ_ZMQCONFIG_H

#if defined(HAVE_CONFIG_H)
#include "config/placeh-config.h"
#endif

#include <stdarg.h>
#include <string>

#if ENABLE_ZMQ
#include <zmq.h>
#endif

#include "primitives/block.h"
#include "primitives/transaction.h"

void zmqError(const char *str);

#endif // PLACEH_ZMQ_ZMQCONFIG_H
