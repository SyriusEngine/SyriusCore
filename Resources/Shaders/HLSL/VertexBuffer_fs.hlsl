struct FSin{
    float4 position: SV_Position;
    float4 color: Color;
};

struct FSout{
    float4 color: SV_Target0;
};

FSout main(FSin input){
    FSout output;
    output.color = input.color;
    return output;
}
