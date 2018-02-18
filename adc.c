#include "adc.h"
#include "gpio.h"

void adcInit(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ADC1 Periph clock enable
	ADC_StructInit(&ADC_InitStructure); //Initialize ADC structure
	// Configure the ADC1 in continuous mode with a resolution equal to 12 bits
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC1->CFGR1 |= ADC_CFGR1_DISCEN; //!!! Software trigger Enable
	ADC_ChannelConfig(ADC1, ADC0_SENS | ADC1_BAT, ADC_SampleTime_239_5Cycles);
	ADC_GetCalibrationFactor(ADC1); //ADC Calibration
	ADC_Cmd(ADC1, ENABLE); //Enable ADC1
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN))
		; //Wait the ADCEN flag
}

uint16_t getAdcValue() {
	ADC_StartOfConversion(ADC1);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		;
	return ADC_GetConversionValue(ADC1);
}
