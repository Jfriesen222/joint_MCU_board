function jointUpdate(vec,joint1,jointCommandPlot1,jointDynamicsPlot1,pStruct1)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

%create some persistent variables for objects and structs
persistent joint jointCommandPlot pStruct i s
if nargin>1
    joint = joint1;
    jointCommandPlot = jointCommandPlot1;
    jointDynamicsPlot = jointDynamicsPlot1;
    pStruct = pStruct1;
    getStaticTensions(joint,10);
    i = 0;
    
    %     myData = uint8('$-223573,-223573,-223573,-223573,-223573,-223573*2C');
    %     fileID = fopen('restlengths.dat','w');
    %     fwrite(fileID,myData,'uint8');
    %     fclose(fileID);
    %     s = memmapfile('restlengths.dat',...
    %         'Format','uint8',...
    %         'Writable',true);
    
    %%%%%%%%%%%%% open serial port %%%%%%%%%%%%%%
            s = serial('COM6','BaudRate',460800,'Timeout',1);
            fopen(s);
            s.BytesAvailableFcnMode = 'terminator';
            s.BytesAvailableFcn = @mycallbacks;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %myData = int32([1:10000]);
    
end
i = i+1;
%%%%%%%%%%%%%%%%%% update Variables %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% set variables from persistent structure
r1 = pStruct.r1; h1 = pStruct.h1; r2 = pStruct.r2; h2 = pStruct.h2; NR = pStruct.NR; minQ = pStruct.minQ; tspan = pStruct.tspan;

% set variables from sliders below
T = vec(1); G = vec(2); P = vec(3);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%% compute quaternions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
quat = dcm2quat(getHG_Tform(T,G,P));
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%% update spine nodes in command plot and spine object %%%%%%%%%%%%%%%%%%%%%
jointNodes = get3DOFJointNodes(h1,r1,r2,h2,NR,quat);
joint.nodePoints = jointNodes;
jointCommandPlot.nodePoints = jointNodes;
tensions = getStaticTensions(joint,60);
restLengths = joint.simStruct.stringRestLengths(4:end);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%send Command%%%%%%%%%%%%%%%%%


% restLengthsbyte_1 =  bitshift(bitand(restLengthsInt32,uint32(hex2dec('ff000000'))),-24);
% restLengthsbyte_2 =  bitshift(bitand(restLengthsInt32,uint32(hex2dec('00ff0000'))),-16);
% restLengthsbyte_3 =  bitshift(bitand(restLengthsInt32,uint32(hex2dec('0000ff00'))),-8);
% restLengthsbyte_4 = bitand(restLengthsInt32,uint32(hex2dec('000000ff')));
% bits = [uint8(restLengthsbyte_1); uint8(restLengthsbyte_2); uint8(restLengthsbyte_3); uint8(restLengthsbyte_4) ; uint8(',,,,,,')];
b = [-397770.190476191,-395251.571428572,-414110.904761905,-409412.333333333,-405040.095238095,-404712.190476191;
      1014571.42857143,1022714.28571429,1016714.28571429,1034333.33333333,1018952.38095238,1012571.42857143];
 restLengthsInt32 = zeros(1,6);  
for ii = 1:6  
  restLengthsInt32(ii) = round(b(1,ii) + b(2,ii)*restLengths(ii));
end
restLengthsInt32 = int32([restLengthsInt32 round(vec(4))]);
bits = typecast(restLengthsInt32,'uint8');
%bits
bits(4:4:end) = [];
nmeaSentence = [uint8('$'), bits];
nmeaSentence = [nmeaSentence , uint8('*')];
checksum = uint8(0);
for count = 1:length(bits)       % checksum calculation ignores $ at start
    checksum = bitxor(checksum,bits(count));  % checksum calculation
end
checksum = dec2hex(checksum,2);
nmeaSentence = [nmeaSentence ,uint8(checksum), uint8(13),  uint8(10)];
%char(nmeaSentence)
% s.data = uint8(nmeaSentence);
 %disp(nmeaSentence);
% disp(s.data)
fwrite(s, nmeaSentence);
flushoutput(s);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%Compute Command and update dynamics if feasible command is generated%%%%%%%

%disp(joint.simStruct.stringRestLengths);
%dynamicsUpdate(joint,tspan);
%jointDynamicsPlot.nodePoints = joint.ySim(1:end/2,:);
%disp(jointDynamicsPlot.nodePoints)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if mod(i,1) == 0
    updatePlot(jointCommandPlot);
    %updatePlot(jointDynamicsPlot);
    drawnow %plot it up
end
end

