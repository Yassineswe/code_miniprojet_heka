from setuptools import setup

package_name = 'turtle_executor'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Yassine Chahlaoui',
    maintainer_email='mohamed-yassine.chahlaoui@etu.polymtl.ca',
    description='Node "tortue" (Python) : traduit les commandes du node cerveau en Twist',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'executor_node = turtle_executor.executor_node:main',
        ],
    },
)
