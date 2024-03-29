/*
f(16,0,0) = ( 0, 1)
f(16,0,1) = ( 2, 3)
f(16,0,2) = ( 4, 5)
f(16,0,3) = ( 6, 7)
f(16,0,4) = ( 8, 9)
f(16,0,5) = (10,11)
f(16,0,6) = (12,13)
f(16,0,7) = (14,15)

f(16,1,0) = ( 1, 2)
f(16,1,1) = ( 1, 3)
f(16,1,2) = ( 5, 6)
f(16,1,3) = ( 5, 7)
f(16,1,4) = ( 9,10)
f(16,1,5) = ( 9,11)
f(16,1,6) = (13,14)
f(16,1,7) = (13,15)

f(16,2,0) = ( 3, 4)
f(16,2,1) = ( 3, 5)
f(16,2,2) = ( 3, 6)
f(16,2,3) = ( 3, 7)
f(16,2,4) = (11,12)
f(16,2,5) = (11,13)
f(16,2,6) = (11,14)
f(16,2,7) = (11,15)

f(x,y,z) = (?,?)

fa(16,0,x) = floor(x/1)*(2^1)
fa(16,1,x) = floor(x/2)*(2^2) + 2^1 - 1
fa(16,2,x) = floor(x/4)*(2^3) + 2^2 - 1
*/


function f(a,b,c){
    var p2b = 1 << b;
    return [
        (c >> b << (b+1)) + (1<<b) - 1,
        (c >> b << b) + c+1 + (b*(b+1)>>1)
        //(c >> b << b) + c+1 + floor(b*(b+1)/2)
    ];
};


//[[i//2**j*2**j + i+1 + j*(j+1)//2 for i in range(8)] for j in range(3)]

floor = Math.floor;
pow = Math.pow;
for (var j=0; j<3; ++j)
    for (var i=0; i<8; ++i)
        console.log(
            i,
            j,
            floor(i/pow(2,j))*pow(2,j) + i+1 + floor(j*(j+1)/2));

for (var y=0; y<3; ++y)
    for (var z=0; z<8; ++z)
        console.log("f(16,"+y+","+z+") = "+JSON.stringify(f(16,y,z)));

