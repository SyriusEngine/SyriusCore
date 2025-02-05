struct FSin{
    float4 position: SV_Position;
    matrix<float, 3, 3> color: Color;
};

struct FSout{
    float3 color: SV_Target0;
};

FSout main(FSin input){
    FSout output;
    output.color = input.color[0];
    return output;
}
