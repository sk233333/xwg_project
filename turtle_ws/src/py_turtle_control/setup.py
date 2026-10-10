from setuptools import setup
#setup() 是 Python官方打包工具 setuptools 的核心函数，ROS2的colcon build底层就是在调用它

package_name = 'py_turtle_control'
#必须和目录名、package.xml 里的 <name> 一致

#所有配置从这里开始，下面每一个参数都是 setup() 的关键字参数
setup(
    name=package_name,
    version='0.0.0',
    #name-包名（ROS 2 和 Python 都认这个），version-版本号（ROS 2 默认 0.0.0，可改）

    packages=[package_name],
    #  告诉 setuptools：哪些目录是 Python 包​，即内层的： 'py_turtle_control'

    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    #data_files 是 setuptools 的参数
    #data_files=[ (安装目标路径, [源文件路径1, 源文件路径2, ...]), (安装目标路径, [源文件路径1, ...]),
    #['resource/'+package_name]='resource/py_turtle_control, 这个文件是ros2 pkg create自动生成的，没有这个标记文件就检测不到你的包。
    #['package.xml']是 ROS2包的身份证，没有 package.xml安装到share下 → ROS2运行时找不到包的元数据


    install_requires=['setuptools'],
    #作用： 声明 Python 依赖，因为 setup.py本身就是 setuptools脚本，如果用了 numpy、yaml (pip 安装的第三方库要加到列表里，内置如math不用

    zip_safe=True,
    #告诉 setuptools：这个包可以安全以 zip形式运行，一般默认即可

    maintainer='you',
    maintainer_email='you@example.com',
    description='用 Python 控制海龟转圈并转发状态',
    license='Apache-2.0',
    #元数据，分别是维护者名字，联系方式，ros2 pkg xml显示的描述，开源协议（不影响运行，但 package.xml 里也要有对应字段，两边保持一致

    entry_points={
        'console_scripts': [
            # 左边是终端命令名（ros2 run 时用的命令名），右边是 内层包名+python代码名:调用的函数名
            'turtle_circle = py_turtle_control.turtle_circle:main',
        ],
    },
)
