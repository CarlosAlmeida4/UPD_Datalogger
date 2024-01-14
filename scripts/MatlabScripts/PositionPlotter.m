data_s = yaml.loadFile("test.yaml");
%%
WheelSpeedBL = cell2mat(data_s.WheelSpeedBL);
WheelSpeedBR = cell2mat(data_s.WheelSpeedBR);
WheelSpeedFL = cell2mat(data_s.WheelSpeedFL);
WheelSpeedFR = cell2mat(data_s.WheelSpeedFR);


subplot(4,1,1);
plot(WheelSpeedFR)

subplot(4,1,2);
plot(WheelSpeedFL)


subplot(4,1,3);
plot(WheelSpeedBL)


subplot(4,1,4);
plot(WheelSpeedBR)