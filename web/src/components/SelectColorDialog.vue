<script setup lang="ts">
import Dialog from './Dialog.vue';
import Row from './Row.vue';
import Column from './Column.vue';
import { reactive } from 'vue';
import { UiSelectColorDialog } from '@/Ui/Dialog/UiSelectColorDialog';
import { FInterface } from '@/Core/FInterface';

const props = defineProps<{ data: UiSelectColorDialog }>()


interface IColorState {
  rgb: { R: number, G: number, B: number };
  colorName: string;
  colorIndex: number;
}

/**
 * 页面参数
 */
const page: {
  ACIColors: Array<Array<IColorState>>
  commonColors: Array<IColorState>,
  ACIColorsText: string,
  RGBColorsText: string,
  colorRegionHeight: number,
  curColorRGB: { r: number, g: number, b: number },
  curColorName: string,
  disableLayerInput: boolean,
  disableBlockInput: boolean,
} = reactive({
  ACIColors: [],
  commonColors: [],
  ACIColorsText: "",
  RGBColorsText: "",
  colorRegionHeight: props.data.height() - 50,
  curColorRGB: { r: 0, g: 0, b: 0 },
  curColorName: "",
  disableLayerInput: props.data.isDisableLayerInput(),
  disableBlockInput: props.data.isDisableBlockInput(),
});




// 添加ACI颜色
{
  let colors: Array<IColorState> = new Array<IColorState>();
  for (let i = 18; i <= 248;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i,
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);

  colors = [];
  for (let i = 16; i <= 246;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);

  colors = [];
  for (let i = 14; i <= 244;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);

  colors = [];
  for (let i = 12; i <= 242;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);

  colors = [];
  for (let i = 10; i <= 240;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);


  colors = [];
  for (let i = 11; i <= 241;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);

  colors = [];
  for (let i = 13; i <= 243;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);

  colors = [];
  for (let i = 15; i <= 245;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);

  colors = [];
  for (let i = 17; i <= 247;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);

  colors = [];
  for (let i = 19; i <= 249;) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    colors.push(colorState);
    i = i + 10;
  }
  page.ACIColors.push(colors);
}

// 添加常用颜色
{
  for (let i = 1; i <= 9; i++) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    if (i == 1) {
      colorState.colorName = "红";
    } else if (i == 2) {
      colorState.colorName = "黄";
    } else if (i == 3) {
      colorState.colorName = "绿";
    } else if (i == 4) {
      colorState.colorName = "青";
    } else if (i == 5) {
      colorState.colorName = "蓝";
    } else if (i == 6) {
      colorState.colorName = "洋红";
    } else if (i == 7) {
      colorState.colorName = "白";
    }
    page.commonColors.push(colorState);
  }
  for (let i = 250; i <= 255; i++) {
    let rgb: { r: number, g: number, b: number } = FInterface.colorIndexToRGB(i);
    let colorState: IColorState = {
      rgb: { R: rgb.r, G: rgb.g, B: rgb.b },
      colorName: `${i}`,
      colorIndex: i
    };
    page.commonColors.push(colorState);
  }
}

// 设置默认颜色
if (props.data.colorIndex() >= 0 && props.data.colorIndex() <= 256) {
  page.curColorName = `${props.data.colorIndex()}`;
  page.curColorRGB = FInterface.colorIndexToRGB(props.data.colorIndex());
}
else {
  page.curColorName = "7";
  page.curColorRGB = FInterface.colorIndexToRGB(7);
}

const onclickColor = (item: IColorState) => {
  page.curColorName = item.colorName;
  page.curColorRGB = FInterface.colorIndexToRGB(item.colorIndex);
  console.log("curColorName:" + page.curColorName);
}
const onmouseenter = (e: MouseEvent, item: IColorState) => {
  page.ACIColorsText = item.colorName;
  page.RGBColorsText = `RGB:  ${item.rgb.R},${item.rgb.G},${item.rgb.B}`;
}
const onclickByLayer = () => {
  page.curColorName = "ByLayer";
  page.curColorRGB = { r: 0, g: 0, b: 0 };
}
const onclickByBlock = () => {
  page.curColorName = "ByBlock";
  page.curColorRGB = { r: 0, g: 0, b: 0 };
}
const onmouseout = (e: MouseEvent, item: IColorState) => {
  page.ACIColorsText = "";
  page.RGBColorsText = "";
}
const onclickConfirm = () => {

  let colorIndex: number = -1;
  if (page.curColorName === '红') {
    colorIndex = 1;
  }
  else if (page.curColorName === '黄') {
    colorIndex = 2;
  }
  else if (page.curColorName === '绿') {
    colorIndex = 3;
  }
  else if (page.curColorName === '青') {
    colorIndex = 4;
  }
  else if (page.curColorName === '蓝') {
    colorIndex = 5;
  }
  else if (page.curColorName === '洋红') {
    colorIndex = 6;
  }
  else if (page.curColorName === '白') {
    colorIndex = 7;
  }
  else if (page.curColorName === 'ByLayer' && page.disableLayerInput === false) {
    colorIndex = 256;
  }
  else if (page.curColorName === 'ByBlock' && page.disableBlockInput === false) {
    colorIndex = 0;
  }
  else {

    let isIni: boolean = false;
    for (let i = 0; i < page.curColorName.length; i++) {
      let c = page.curColorName.charAt(i);
      if (c < '0' || c > '9') {
        break;
      }
      if (i == page.curColorName.length - 1) {
        isIni = true;
      }
    }
    if (isIni == false) {
      colorIndex = -1;
    }

    if (isIni == true) {
      colorIndex = parseInt(page.curColorName, 10);
    }
  }

  if (colorIndex < 0 || colorIndex > 256) {
    alert("请输入在 0 - 256 之间的整数值。");
    return;
  }

  props.data.setColorIndex(colorIndex);
  if(props.data.m_funConfirm != null) {
    props.data.m_funConfirm(colorIndex);
  }
  props.data.close();
}
const onclickCancel = () => {
  props.data.close();
}




</script>



<template>

  <Dialog :data="data">
    <Row :style="{ height: '5px' }"></Row>
    <Row>
      <!-- 左边间隔 -->
      <Column :span="2"></Column>
      <!-- 中间内容 -->
      <Column :span="96">
        <!-- 上方颜色栏 -->
        <Row :style="{ height: '295px', backgroundColor: 'rgb(249,249,249)', flexDirection: 'column' }">
          <!-- 选择颜色类型 -->
          <Row>
            <div :style="{
              height: '20px', fontSize: '13px',
              backgroundColor: 'rgb(253,253,253)', borderLeft: '1px solid rgb(186,186,186)',
              borderTop: '1px solid rgb(186,186,186)', borderRight: '1px solid rgb(186,186,186)',
              whiteSpace: 'pre', display: 'flex', justifyContent: 'center', alignItems: 'center'
            }">
              <div :style="{ marginLeft: '5px', marginRight: '5px' }">索引颜色</div>
            </div>
            <div :style="{
              width: '100%', height: '20px',
              borderBottom: '1px solid rgb(186,186,186)', backgroundColor: 'rgb(240,240,240)'
            }">
            </div>
          </Row>
          <!-- 颜色区域 -->
          <Row :style="{
            height: '275px', borderLeft: '1px solid rgb(186,186,186)',
            borderRight: '1px solid rgb(186,186,186)', borderBottom: '1px solid rgb(186,186,186)'
          }">
            <Column :span="2"></Column>
            <Column :span="96">
              <!-- 文字 -->
              <Row :style="{ fontSize: '13px' }">Cad 颜色索引 (ACI)</Row>
              <!-- 颜色 -->
              <Row v-for="(itemRow, nRow) in page.ACIColors" :key="nRow"
                :style="{ height: '13px', marginTop: (nRow === 5) ? '8px' : '0px' }">
                <div v-for="(item, nColumn) in itemRow" :key="nColumn" :style="{
                  width: '18px', height: '13px',
                  display: 'flex', justifyContent: 'center', alignItems: 'center'
                }">
                  <div :style="{
                    width: '15px', height: '12px',
                    backgroundColor: `rgb(${item.rgb.R},${item.rgb.G},${item.rgb.B})`
                  }"  v-on:click="() => { onclickColor(item) }">
                  </div>
                </div>

              </Row>
              <!-- 颜色 -->
              <Row :style="{ fontSize: '12px', height: '20px' }">
                <Column v-if="page.ACIColorsText.length > 0" :span="50">索引颜色: {{ page.ACIColorsText }}</Column>
                <Column v-if="page.RGBColorsText.length > 0" :span="50">{{ page.RGBColorsText }}</Column>
              </Row>
              <!-- 间隔 -->
              <Row :style="{ height: '4px' }"></Row>
              <!-- 常用颜色1 -->
              <Row :style="{ height: '22px' }">
                <!-- 常用颜色 -->
                <Column :span="50" :style="{ width: '100%', height: '100%', flexDirection: '', }">
                  <div v-for="(item, index) in page.commonColors" :key="index" :style="{
                    display: index >= 9 ? 'none' : 'flex', width: '18px', height: '100%',
                    backgroundColor: 'rgb(244,244,244)', justifyContent: 'flex', alignItems: 'center'
                  }">
                    <div v-if="index < 9" :style="{
                      width: '16px', height: '16px', marginRight: '2px',
                      backgroundColor: `rgb(${item.rgb.R},${item.rgb.G},${item.rgb.B})`
                    }" v-on:mouseenter="($ele) => { onmouseenter($ele, item) }"
                      v-on:mouseout="($ele) => { onmouseout($ele, item) }" v-on:click="() => { onclickColor(item) }">
                    </div>
                  </div>
                </Column>
                <!-- ByLayer按钮和ByBlock按钮 -->
                <Column :span="50" :style="{ width: '100%', justifyContent: 'flex-end', flexDirection: '' }">
                  <!-- ByLayer(L) -->
                  <div v-if="page.disableLayerInput === false" :style="{
                    height: '20px', backgroundColor: 'rgb(255,255,255)',
                    display: 'flex', justifyContent: 'center', borderRadius: '5px', border: '1px solid rgb(186,186,186)'
                  }" v-on:click="onclickByLayer">
                    <div :style="{ marginLeft: '5px', marginRight: '5px', fontSize: '80%' }">ByLayer(L)</div>
                  </div>
                  <!-- ByBlock(B) -->
                  <div v-if="page.disableLayerInput === false" :style="{
                    height: '20px', marginLeft: '10px', backgroundColor: 'rgb(255,255,255)',
                    display: 'flex', justifyContent: 'center', border: '1px solid rgb(186,186,186)', borderRadius: '5px'
                  }" v-on:click="onclickByBlock">
                    <div :style="{ marginLeft: '5px', marginRight: '5px', fontSize: '80%' }">ByBlock(B)</div>
                  </div>
                </Column>
              </Row>
              <!-- 间隔 -->
              <Row :style="{ height: '4px' }"></Row>
              <!-- 常用颜色2和输入框 -->
              <Row>
                <Column :span="55">
                  <!-- 常用颜色2 -->
                  <Row>
                    <div v-for="(item, index) in page.commonColors" :key="index">
                      <div v-if="index > 9" :style="{
                        width: '16px', height: '16px', marginRight: '2px',
                        backgroundColor: `rgb(${item.rgb.R},${item.rgb.G},${item.rgb.B})`
                      }" v-on:mouseenter="($ele) => { onmouseenter($ele, item) }"
                        v-on:mouseout="($ele) => { onmouseout($ele, item) }" v-on:click="() => { onclickColor(item) }">
                      </div>
                    </div>
                  </Row>
                  <!-- 间隔 -->
                  <Row :style="{ height: '5px' }"></Row>
                  <!-- 颜色文字 -->
                  <Row :style="{ fontSize: '12px' }">颜色(C):</Row>
                  <!-- 颜色值 -->
                  <Row :style="{ fontSize: '12px' }">
                    <input v-model="page.curColorName" :style="{
                      width: '100%', height: '14px', outline: 'none',
                      borderLeft: '1px solid rgb(236,236,236)', borderRight: '1px solid rgb(236,236,236)',
                      borderTop: '1px solid rgb(236,236,236)', borderBottom: '2px solid rgb(0,103,192)'
                    }" />
                  </Row>
                </Column>
                <Column :span="45" :style="{ justifyContent: 'center', alignItems: 'center' }">
                  <div :style="{
                    width: '42px', height: '42px', border: '1px solid rgb(186,186,186)'
                  }">
                    <div :style="{
                      width: '40px', height: '40px', margin: '1px',
                      backgroundColor: `rgb(${page.curColorRGB.r},${page.curColorRGB.g},${page.curColorRGB.b})`
                    }">
                    </div>
                  </div>
                </Column>
              </Row>
            </Column>
            <Column :span="2"></Column>
          </Row>
        </Row>
        <!-- 下方按钮确定和取消按钮 -->
        <Row :style="{ height: '10px' }"></Row>
        <Row :style="{ height: '30px', justifyContent: 'flex-end' }">
          <!-- 确定 -->
          <div v-if="page.disableLayerInput === false" :style="{
            height: '20px', backgroundColor: 'rgb(255,255,255)',
            display: 'flex', justifyContent: 'center', borderRadius: '5px', border: '1px solid rgb(186,186,186)'
          }" v-on:click="onclickConfirm">
            <div :style="{ marginLeft: '30px', marginRight: '30px', fontSize: '80%' }">确定</div>
          </div>
          <!-- 取消 -->
          <div v-if="page.disableLayerInput === false" :style="{
            height: '20px', marginLeft: '10px', backgroundColor: 'rgb(255,255,255)',
            display: 'flex', justifyContent: 'center', border: '1px solid rgb(186,186,186)', borderRadius: '5px'
          }" v-on:click="onclickCancel">
            <div :style="{ marginLeft: '30px', marginRight: '30px', fontSize: '80%' }">取消</div>
          </div>
        </Row>
      </Column>
      <!-- 右边间隔 -->
      <Column :span="2"></Column>
    </Row>
    <Row :style="{ height: '5px' }"></Row>
  </Dialog>

</template>
