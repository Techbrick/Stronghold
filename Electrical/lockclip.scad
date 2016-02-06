sample();

module sample(){
    difference(){
        union(){
            translate([0,0,10/2])
            cube([77, 8, 10],center=true);
            translate([0,0,10-3.3/2])
            cube([77,12, 3.3],center=true);
            translate([0,0,.2])
            cube([85,16,.4],center=true);
        }
        for(x = [0:1:9]){
            translate([-5*7.62+7.62/2+x*7.62,0,7.7])
            rotate([-90,0,90])
            3pin();
        }
        for(y = [-6,6]){
            translate([0,y,10-3.3])
            rotate([45,0,0])
            #cube([80,sqrt(2*(12-8)),sqrt(2*(12-8))],center=true);
        }
    }
}

module roborioright(){
    
}

//translate([25,0,0])
//3pin();


module 3pin(){
    //scale(25.4)
    //import("D:\\Downloads\\Molex-CGrid-SL-3\\050579403_fixed.stl", convexity=10);
    cube([8.5,20,3.3],center=true);
    translate([-2.54/2,0,(2.54+1.2)/2])
    cube([1.2, 20, 1.5], center=true);
    translate([0,0,-1.4])
    cube([3, 20, 3], center=true);
    translate([0,-5.5, -2.8])
    cube([8.5,9,3.2], center=true);
    translate([0,-0,-2.1])
    cube([5,4,2], center=true);
    translate([0,1.2,-2])
    cube([2.5,1.5,4],center=true);
}