struct Tracker{
    static long long tile_res_time ;
    static long long tile_construct_time ;
    static long long tile_mul_time ;
    static long long tile_fill_time ;
    static long long tile_load_time;
    static std::chrono::steady_clock::time_point begin;
    static std::chrono::steady_clock::time_point end;
};

#define TIME_ELAPSE_START() \
    Tracker::begin = std::chrono::steady_clock::now();
#define TIME_ELAPSE_END(VAR) \
    Tracker::end = std::chrono::steady_clock::now(); \
    VAR += std::chrono::duration_cast<std::chrono::nanoseconds>(Tracker::end - Tracker::begin).count();
