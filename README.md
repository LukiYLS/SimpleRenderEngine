# SimpleRenderEngine
​	这是我学习图形学的过程中，积累的一些东西，然后放到一个框架下，就形成了一个简单的渲染器，正是由于这个想法，在我实现一些图形效果是会尽可能考虑如何更好的融入到框架之下，这可能会让我花费大量的时间去考虑框架设计的一些问题，确实如此，比如这个第一版，我就花了三个月（工作之外的业余时间），不断的重构，去思考如何才能越写越简单好用，而不是更复杂。由于没有太多经验，我可能对于如何设计高耦合、低内聚的程序，没有很深的理解，我在写的过程中就思考一个问题，假如有人想用这个渲染器去调试一些效果，如何让一个图形学初学者就能很快速简单的拼凑出它想要的效果，并且能在这之上添加自己的东西也足够简单。在这个过程中，可能会大量参考开源引擎，并尽量移植过来，我觉得这也是一条非常好的学习途径。更多关于这个项目的一些想法会写到个人博客上：http://yanglusheng.com/

## V1.0功能模块



###  1. 基础库

- Math库：Vector,Matrix,Frustum,Quaternion

- HardwareBuffer：硬件缓冲区管理顶点、索引、纹理等资源

- Utils：事件驱动管理以及相机控制器

- GeometryFactory，MeshLoader

- Object：所有的Actor都继承Object

- ResourceManager：Texture、Shader....

  不一而足

### 2. 对材质的封装，材质和shader对应，

- PhongMaterial,带光照的材质（包含Shadow）

DirectionLight

![DirectionLight](http://ovi6hpv55.bkt.clouddn.com/Light_ShadowMap.png)

PointLight

![PointLight](http://ovi6hpv55.bkt.clouddn.com/point.png)

SpotLight

![SpotLight](http://ovi6hpv55.bkt.clouddn.com/spot.png)



- BasicMaterial不带光照的材质

- ShaderMaterial自定义shader的材质

  ​

![Earth](http://ovi6hpv55.bkt.clouddn.com/earth.png)

### 3.鼠标控制器 



###  4.TerrianTile



![Terrian](http://ovi6hpv55.bkt.clouddn.com/terrian.png)





