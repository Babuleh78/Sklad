class TimeMap {
public:
    unordered_map<string, map<int, string>> MainMap;//Имя время состояние
    TimeMap() {
        
    }
    
    void set(string key, string value, int timestamp) {
        // map<int, string> help;
        // help[timestamp] = value;
        // MainMap[key] = help;
        MainMap[key].insert({timestamp, value});
    }
    
    string get(string key, int timestamp) {
        auto it = MainMap[key].upper_bound(timestamp);
        return it == MainMap[key].begin() ? "" : prev(it)->second;
    }
};
