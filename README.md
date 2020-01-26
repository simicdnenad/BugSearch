Bugce

Bugfixing/upgrading of Bugce.

Upgrading Bugce:
1. Add auto f1 = std::async(&Test::calculate, this, 11, 20);
       auto res1 = f1.get();
   for fetching results from thread. Try (only for training) std::future and std::promise things...
2. Add static fields for loading bug and landscape one time instead of multiple times...

Fixed:
Bug.cpp:98, On Linux string::npos (circa 10pow8) don't match std::string::find() return value if string not find (-1); On Windows (VS) this is working ok.
