# KDSingleApplication

A helper class for single-instance policy applications.

# Documentation

Woefully lacking, but see the examples or tests for inspiration. Basically it involves:

1. Create a `Q(Core|Gui)Application` object.
2. Create a `KDSingleApplication` object.
3. Check if the current instance is *primary* (or "master") or *secondary* (or "slave") by calling `isPrimaryInstance`:
    * the *primary* instance needs to listen from messages coming from the secondary instances, by connecting a slot to the `messageReceived` signal;
    * the *secondary* instances can send messages to the primary instance by calling `sendMessage`.

# Licensing

This software is provided under the MIT license (see LICENSE.MIT.txt). Contact KDAB at <info@kdab.com> if you need different licensing options.
