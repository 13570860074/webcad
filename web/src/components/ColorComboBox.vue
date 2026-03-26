<script setup lang="ts">
import { reactive, onMounted, onBeforeUnmount } from 'vue';
import Row from './Row.vue';
import Arx from '@/kernel/arxHeaders';
import Db from '@/kernel/Db';
import Cm from '@/kernel/Cm';
import Ap from '@/kernel/Ap';
import Scrollbar from './Scrollbar.vue';
import { gcadGlobal_uiLayoutData } from '@/gcadGlobal';
import { application } from '@/Application/FApplication';
import { EventState } from '@/EventManager/FEventManager';
import { UiSelectColorDialog } from '@/Ui/Dialog/UiSelectColorDialog';
import ModelBox from './ModelBox.vue';



onMounted(() => {
  let element = document.getElementById(attributeComboBoxElementId);
  if (element != null) {
    page.width = element.getBoundingClientRect().width;
    page.height = element.getBoundingClientRect().height;
  }
  init();
});
onBeforeUnmount(() => {

});


interface IColorState {
  bSelected?: boolean;
  bHighlight?: boolean;
  rgb: { r: number, g: number, b: number };
  colorName?: string;
  colorIndex?: number;
}


/**
 * 页面参数
 */
const page: {
  width: number,
  height: number,

  ACIColors: Array<Array<IColorState>>
  commonColors: Array<IColorState>,

  bExpand: boolean,

  box: {
    left: number,
    top: number,
    right: number,
    bottom: number,
  },
  menubarBox: {
    left: number,
    top: number,
    right: number,
    bottom: number,
  },
} = reactive({
  width: 24,
  height: 24,

  ACIColors: [],
  commonColors: [],

  bExpand: false,

  box: {
    left: 0,
    top: 0,
    right: 0,
    bottom: 0,
  },
  menubarBox: {
    left: 0,
    top: 0,
    right: 0,
    bottom: 0,
  },
});
const attributeComboBoxElementId: string = 'AttributeComboBoxElement';


/** 初始化 */
const init = () => {

  let colors: Array<IColorState> = [];
  colors.push({ rgb: { r: 0, g: 0, b: 0 } });
  colors.push({ rgb: { r: 248, g: 215, b: 49 } });
  colors.push({ rgb: { r: 205, g: 105, b: 40 } });
  colors.push({ rgb: { r: 153, g: 27, b: 30 } });
  colors.push({ rgb: { r: 100, g: 33, b: 101 } });
  colors.push({ rgb: { r: 41, g: 49, b: 137 } });
  colors.push({ rgb: { r: 34, g: 52, b: 110 } });
  colors.push({ rgb: { r: 19, g: 103, b: 52 } });
  colors.push({ rgb: { r: 114, g: 116, b: 48 } });
  page.ACIColors.push(colors);

  colors = [];
  colors.push({ rgb: { r: 0, g: 0, b: 0 } });
  colors.push({ rgb: { r: 248, g: 215, b: 49 } });
  colors.push({ rgb: { r: 205, g: 105, b: 40 } });
  colors.push({ rgb: { r: 153, g: 27, b: 30 } });
  colors.push({ rgb: { r: 100, g: 33, b: 101 } });
  colors.push({ rgb: { r: 41, g: 49, b: 137 } });
  colors.push({ rgb: { r: 34, g: 52, b: 110 } });
  colors.push({ rgb: { r: 19, g: 103, b: 52 } });
  colors.push({ rgb: { r: 114, g: 116, b: 48 } });
  page.ACIColors.push(colors);

  colors = [];
  colors.push({ rgb: { r: 99, g: 100, b: 102 } });
  colors.push({ rgb: { r: 241, g: 235, b: 31 } });
  colors.push({ rgb: { r: 242, g: 103, b: 34 } });
  colors.push({ rgb: { r: 205, g: 32, b: 39 } });
  colors.push({ rgb: { r: 147, g: 39, b: 143 } });
  colors.push({ rgb: { r: 69, g: 84, b: 165 } });
  colors.push({ rgb: { r: 16, g: 86, b: 137 } });
  colors.push({ rgb: { r: 19, g: 155, b: 72 } });
  colors.push({ rgb: { r: 153, g: 155, b: 55 } });
  page.ACIColors.push(colors);

  colors = [];
  colors.push({ rgb: { r: 147, g: 149, b: 152 } });
  colors.push({ rgb: { r: 244, g: 238, b: 81 } });
  colors.push({ rgb: { r: 248, g: 153, b: 30 } });
  colors.push({ rgb: { r: 237, g: 31, b: 36 } });
  colors.push({ rgb: { r: 169, g: 83, b: 160 } });
  colors.push({ rgb: { r: 94, g: 103, b: 175 } });
  colors.push({ rgb: { r: 39, g: 118, b: 187 } });
  colors.push({ rgb: { r: 88, g: 186, b: 72 } });
  colors.push({ rgb: { r: 200, g: 201, b: 45 } });
  page.ACIColors.push(colors);

  colors = [];
  colors.push({ rgb: { r: 199, g: 200, b: 202 } });
  colors.push({ rgb: { r: 247, g: 242, b: 129 } });
  colors.push({ rgb: { r: 234, g: 204, b: 102 } });
  colors.push({ rgb: { r: 242, g: 113, b: 114 } });
  colors.push({ rgb: { r: 209, g: 156, b: 199 } });
  colors.push({ rgb: { r: 152, g: 152, b: 204 } });
  colors.push({ rgb: { r: 122, g: 175, b: 223 } });
  colors.push({ rgb: { r: 175, g: 217, b: 170 } });
  colors.push({ rgb: { r: 229, g: 232, b: 120 } });
  page.ACIColors.push(colors);

  colors = [];
  colors.push({ rgb: { r: 255, g: 255, b: 255 } });
  colors.push({ rgb: { r: 248, g: 246, b: 176 } });
  colors.push({ rgb: { r: 254, g: 234, b: 185 } });
  colors.push({ rgb: { r: 247, g: 171, b: 174 } });
  colors.push({ rgb: { r: 221, g: 179, b: 212 } });
  colors.push({ rgb: { r: 206, g: 204, b: 230 } });
  colors.push({ rgb: { r: 173, g: 221, b: 247 } });
  colors.push({ rgb: { r: 215, g: 235, b: 210 } });
  colors.push({ rgb: { r: 237, g: 238, b: 153 } });
  page.ACIColors.push(colors);


  page.commonColors.push({ rgb: { r: 255, g: 0, b: 0 } });
  page.commonColors.push({ rgb: { r: 255, g: 255, b: 0 } });
  page.commonColors.push({ rgb: { r: 0, g: 255, b: 0 } });
  page.commonColors.push({ rgb: { r: 0, g: 255, b: 255 } });
  page.commonColors.push({ rgb: { r: 0, g: 0, b: 255 } });
  page.commonColors.push({ rgb: { r: 255, g: 0, b: 255 } });
  page.commonColors.push({ rgb: { r: 255, g: 255, b: 255 } });
  page.commonColors.push({ rgb: { r: 128, g: 128, b: 128 } });
  page.commonColors.push({ rgb: { r: 192, g: 192, b: 192 } });

  for (let i = 0; i < page.ACIColors.length; i++) {
    for (let j = 0; j < page.ACIColors[i].length; j++) {
      let color: IColorState = page.ACIColors[i][j];
      color.bSelected = false;
      color.bHighlight = false;
      let cmcolor: Cm.CmColor = new Cm.CmColor();
      cmcolor.setRGB(color.rgb.r, color.rgb.g, color.rgb.b);
      color.colorIndex = cmcolor.colorIndex();
      color.colorName = cmcolor.colorName(); 
      Arx.emsdk_free(cmcolor);
    }
  }
  for (let i = 0; i < page.commonColors.length; i++) {
    let color: IColorState = page.commonColors[i];
    color.bSelected = false;
    color.bHighlight = false;
    let cmcolor: Cm.CmColor = new Cm.CmColor();
    cmcolor.setRGB(color.rgb.r, color.rgb.g, color.rgb.b);
    color.colorIndex = cmcolor.colorIndex();
    color.colorName = cmcolor.colorName(); 
    Arx.emsdk_free(cmcolor);
  }
}
/** 点击展开 */
const onclickExpand = () => {

  /** 展开 */
  page.bExpand = true;
  let element: HTMLElement = document.getElementById('gcad');
  let layerComboBoxElement: HTMLElement = document.getElementById(attributeComboBoxElementId);
  if (element != null && layerComboBoxElement != null) {
    let rect: DOMRect = element.getBoundingClientRect();
    page.box.left = rect.left;
    page.box.top = rect.top;
    page.box.right = rect.right;
    page.box.bottom = rect.bottom;

    page.menubarBox.left = layerComboBoxElement.getBoundingClientRect().left - rect.left;
    page.menubarBox.top = layerComboBoxElement.getBoundingClientRect().bottom - rect.top;
    page.menubarBox.right = layerComboBoxElement.getBoundingClientRect().right - rect.left;
  }
}
/** 点击关闭展开 */
const onclickCloseExpand = () => {
  page.bExpand = false;
}
/** 点击颜色 */
const onclickColor = async (layer: { name: string, colorIndex: number, isLocked: boolean, isFrozen: boolean, isOff: boolean }) => {
  let dialog: UiSelectColorDialog = new UiSelectColorDialog();
  dialog.setTitle('选择颜色');
  dialog.setColorIndex(layer.colorIndex);
  dialog.onConfirm((colorIndex: number) => {
    if (colorIndex <= 0 || colorIndex > 255) {
      return;
    }
    let database: Db.DbDatabase = Arx.acdbHostApplicationServices().workingDatabase();
    if (database == null) {
      return;
    }
    let layerTable: Db.DbLayerTable = database.getLayerTable();
    if (layerTable == null) {
      return;
    }
    let layerTableRecord: Db.DbLayerTableRecord = layerTable.getAt(layer.name, Db.OpenMode.kForWrite);
    if (layerTableRecord == null) {
      return;
    }
    let color: Cm.CmColor = new Cm.CmColor();
    color.setColorIndex(colorIndex);
    layerTableRecord.setColor(color);
    layerTableRecord.close();
    layerTable.close();
    Arx.emsdk_free(color);
  });
  dialog.exec();
}
/** 鼠标移入菜单栏 */
const onmouseenter = (item: { bHighlight: boolean }) => {

}


</script>

<template>
  <Row :style="{ height: '100%', alignItems: 'center' }">
    <!-- 下拉条 -->
    <div :id="attributeComboBoxElementId" :style="{
      width: '100%', height: '100%', display: 'flex', justifyContent: 'center', alignItems: 'center'
    }" v-on:click="onclickExpand">
      <Row :style="{
        position: 'relative', width: '100%', height: '80%',
        backgroundColor: 'rgb(78,90,110)', border: '1px solid rgb(42,48,59)',
        justifyContent: 'flex', alignItems: 'center',
      }">
        <!-- 颜色 -->
        <div :style="{
          width: `${page.height * 0.6}px`, height: `${page.height * 0.6}px`, marginLeft: '5px',
          backgroundColor: 'rgb(255,255,255)', border: '1px solid rgb(0,0,0)'
        }"></div>
        <!-- 文字 -->
        <div :style="{ marginLeft: '5px', fontSize: '70%', color: 'rgb(255,255,255)' }">ByLayer</div>
        <!-- 下拉 -->
        <div :style="{
          position: 'absolute', width: '100%', height: '100%', marginLeft: '5px', fontSize: '100%',
          color: 'rgb(255,255,255)', display: 'flex', justifyContent: 'flex-end', alignItems: 'center'
        }">
          <div :style="{}">▾</div>
          <div :style="{ width: '10px' }"></div>
        </div>
      </Row>
    </div>
  </Row>

  <!-- 展开菜单 -->
  <ModelBox v-if="page.bExpand === true" :style="{ position: 'fixed' }" v-on:click="onclickCloseExpand">

    <Row :style="{
      width: `${page.menubarBox.right - page.menubarBox.left + 60}px`,
      maxHeight: `${(gcadGlobal_uiLayoutData().canvasBottom - gcadGlobal_uiLayoutData().canvasTop) * 0.6}px`,
      backgroundColor: 'rgb(78,90,110)', marginLeft: `${page.menubarBox.left}px`, marginTop: `${page.menubarBox.top}px`,
      border: '1px solid rgb(0,0,0)', flexDirection: 'column'
    }">
      <!-- 间隔 -->
      <Row :style="{ height: '5px' }"></Row>
      <!-- 图层 -->
      <Row :style="{
        width: '100%', backgroundColor: 'rgb(78,90,110)', justifyContent: 'flex', alignItems: 'center',
      }">
        <!-- 颜色 -->
        <div :style="{
          width: `${page.height * 0.6}px`, height: `${page.height * 0.6}px`, marginLeft: '5px',
          backgroundColor: 'rgb(255,255,255)', border: '1px solid rgb(0,0,0)'
        }"></div>
        <!-- 文字 -->
        <div :style="{ marginLeft: '5px', fontSize: '70%', color: 'rgb(255,255,255)' }">ByLayer</div>
      </Row>
      <!-- 间隔 -->
      <Row :style="{ height: '5px' }"></Row>
      <!-- 块 -->
      <Row :style="{
        width: '100%', backgroundColor: 'rgb(78,90,110)', justifyContent: 'flex', alignItems: 'center',
      }">
        <!-- 颜色 -->
        <div :style="{
          width: `${page.height * 0.6}px`, height: `${page.height * 0.6}px`, marginLeft: '5px',
          backgroundColor: 'rgb(255,255,255)', border: '1px solid rgb(0,0,0)'
        }"></div>
        <!-- 文字 -->
        <div :style="{ marginLeft: '5px', fontSize: '70%', color: 'rgb(255,255,255)' }">ByLayer</div>
      </Row>
      <!-- 间隔 -->
      <Row :style="{ height: '5px', borderBottom: '1px solid rgb(0,0,0)' }"></Row>
      <Row :style="{ height: '5px' }"></Row>
      <!-- 颜色 -->
      <Row :style="{ flexDirection: 'column' }">
        <Row v-for="(item) in page.ACIColors" :style="{ justifyContent: 'center', alignItems: 'center' }">
          <div v-for="(color) in item" :style="{
            width: '21px', height: '21px', display: 'flex', justifyContent: 'center', alignItems: 'center'
          }">
            <div :style="{
              width: '16px', height: '16px',
              backgroundColor: `rgb(${color.rgb.r},${color.rgb.g},${color.rgb.b})`, border: '1px solid rgb(171,171,171)'
            }">
            </div>
          </div>
        </Row>
      </Row>
      <Row :style="{ height: '5px', borderBottom: '1px solid rgb(0,0,0)' }"></Row>
      <!-- 索引颜色 -->
      <Row :style="{ color: 'rgb(255,255,255)', fontSize: '80%' }">
        <div :style="{ marginLeft: '5px' }">索引颜色</div>
      </Row>
      <!-- 常用颜色 -->
      <Row :style="{ justifyContent: 'center', alignItems: 'center' }">
        <div v-for="(color) in page.commonColors" :style="{
          width: '21px', height: '21px', display: 'flex', justifyContent: 'center', alignItems: 'center'
        }">
          <div :style="{
            width: '16px', height: '16px',
            backgroundColor: `rgb(${color.rgb.r},${color.rgb.g},${color.rgb.b})`, border: '1px solid rgb(171,171,171)'
          }">
          </div>
        </div>
      </Row>
      <!-- 间隔 -->
      <Row :style="{ height: '5px', borderBottom: '1px solid rgb(0,0,0)' }"></Row>
      <Row :style="{ height: '5px' }"></Row>
      <!-- 更多颜色 -->
      <Row :style="{ justifyContent: 'flex', alignItems: 'center' }">
        <img :src="'./resources/svg/color.svg'"
          :style="{ width: `${page.height * 0.6}px`, height: `${page.height * 0.6}px`, marginLeft: '5px' }"></img>
        <div :style="{ marginLeft: '4px', color: 'rgb(255,255,255)', fontSize: '80%', }">更多颜色</div>
      </Row>
      <!-- 间隔 -->
      <Row :style="{ height: '5px' }"></Row>
    </Row>

  </ModelBox>

</template>
