typedef std::vector<uint8_t> bytes;
typedef std::variant<int, std::string> MyAny;
typedef std::unordered_map<MyAny, MyAny> MyMap;
typedef std::variant<MyMap, std::string, bytes> MyReturnType;
typedef std::function<MyReturnType()> MyTask;