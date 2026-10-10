# 海龟状态报告（我们自己定义的数据格式）
# 规则：文件名首字母大写驼峰 + .msg；一行一个字段；# 是注释
# 这个文件会被 rosidl 自动翻译成 Python 类和 C++ 头文件，
# 所以 Python 和 C++ 能"说同一种语言"

string turtle_name          # 海龟名字
float64 x                   # x 坐标
float64 y                   # y 坐标
float64 theta               # 朝向
float64 linear_speed        # 当前前进速度
float64 angular_speed       # 当前转弯速度
builtin_interfaces/Time stamp   # 时间戳
#builtin_interfaces 是 ROS 2 官方提供的内置包，专门存放基础时间类型。
#Time 是这个包里的一个标准消息，它本身只包含两个部分：int32 sec（秒）和 uint32 nanosec（纳秒），精度极高。
#stamp：这是你定义的字段名称（变量名）。
