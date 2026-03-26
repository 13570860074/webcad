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
  let element = document.getElementById(layerComboBoxElementId);
  if (element != null) {
    page.width = element.getBoundingClientRect().width;
    page.height = element.getBoundingClientRect().height;
  }
  init();
});
onBeforeUnmount(() => {
  console.log('onBeforeUnmount');
});

/**
 * 页面参数
 */
const page: {
  width: number,
  height: number,

  bExpand: boolean,
  layers: Array<{
    name: string,
    isLocked: boolean,
    isFrozen: boolean,
    isOff: boolean,
    bCurrent: boolean,
    bHighlight: boolean,
    colorIndex: number,
    color: { r: number, g: number, b: number }
  }>,

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

  bExpand: false,
  layers: [],

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
const layerComboBoxElementId: string = 'LayerComboBoxElement';


/** 初始化 */
const init = () => {
  application.eventManager().on(EventState.kDocumentActivated, (pActivatingDoc: Ap.ApDocument) => {
    console.log(pActivatingDoc);
  });
}
/** 点击展开 */
const onclickExpand = () => {

  /** 获得图层对象 */
  let database: Db.DbDatabase = Arx.acdbHostApplicationServices().workingDatabase();
  if (database == null) {
    return;
  }
  let layerTable: Db.DbLayerTable = database.getLayerTable();
  if (layerTable == null) {
    return;
  }
  let layerTableIterator: Db.DbLayerTableIterator = layerTable.newIterator();
  layerTable.close();

  /** 获得当前图层 */
  let clayerId: Db.DbObjectId = database.clayer();

  /** 获得所有图层对象 */
  page.layers = [];
  for (layerTableIterator.start(); !layerTableIterator.done(); layerTableIterator.step()) {
    let layerTableRecord: Db.DbLayerTableRecord = layerTableIterator.getRecord();
    if (layerTableRecord == null) {
      continue;
    }

    let name: string = layerTableRecord.getName();
    let isLocked: boolean = layerTableRecord.isLocked();
    let isFrozen: boolean = layerTableRecord.isFrozen();
    let isOff: boolean = layerTableRecord.isOff();
    let bCurrent: boolean = false;
    if (clayerId.asOldId() === layerTableRecord.objectId().asOldId()) {
      bCurrent = true;
    }
    let color: Cm.CmColor = layerTableRecord.color();
    let rgb: { r: number, g: number, b: number } = { r: color.red(), g: color.green(), b: color.blue() };
    page.layers.push({
      name: name,
      isLocked: isLocked,
      isFrozen: isFrozen,
      isOff: isOff,
      color: rgb,
      bCurrent: bCurrent,
      bHighlight: false,
      colorIndex: color.colorIndex(),
    });
    layerTableRecord.close();
  }
  Arx.emsdk_free(layerTableIterator);

  /** 展开 */
  page.bExpand = true;
  let element: HTMLElement = document.getElementById('gcad');
  let layerComboBoxElement: HTMLElement = document.getElementById(layerComboBoxElementId);
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
   // Arx.emsdk_free(color);
  });
  dialog.exec();
}
/** 鼠标移入菜单栏 */
const onmouseenter = (item: { bHighlight: boolean }) => {
  for (let i = 0; i < page.layers.length; i++) {
    page.layers[i].bHighlight = false;
  }
  item.bHighlight = true;
}


</script>

<template>
  <!-- 图层栏 -->
  <Row :style="{ height: '100%', alignItems: 'center' }">

    <!-- 图层特性图标 -->
    <img :src="'./resources/svg/layers.svg'" :style="{ width: `${page.height}px`, height: `${page.height}px` }"></img>

    <!-- 图层下拉条 -->
    <div :id="layerComboBoxElementId" :style="{
      width: '100%', height: '100%', marginLeft: '5px',
      display: 'flex', justifyContent: 'center', alignItems: 'center'
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
        <div :style="{ marginLeft: '5px', fontSize: '70%', color: 'rgb(255,255,255)' }">图层1</div>
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

    <!-- 展开按钮 -->
    <img :src="'./resources/svg/expand.svg'" :style="{
      marginLeft: '5px', width: `${page.height}px`, height: `${page.height}px`
    }"></img>
  </Row>

  <!-- 展开菜单 -->
  <ModelBox v-if="page.bExpand === true" :style="{ position: 'fixed' }" v-on:click="onclickCloseExpand">

    <!-- 滚动条 -->
    <Scrollbar :style="{
      width: `${page.menubarBox.right - page.menubarBox.left + 150}px`,
      maxHeight: `${(gcadGlobal_uiLayoutData().canvasBottom - gcadGlobal_uiLayoutData().canvasTop) * 0.6}px`,
      backgroundColor: 'rgb(78,90,110)', marginLeft: `${page.menubarBox.left}px`, marginTop: `${page.menubarBox.top}px`,
      border: '1px solid rgb(0,0,0)'
    }">
      <Row :style="{ height: '3px' }"></Row>
      <Row v-for="layer in page.layers" :key="layer.name" :style="{
        height: `${page.height * 1.2}px`, justifyContent: 'flex', alignItems: 'center',
        backgroundColor: layer.bCurrent ? ('rgb(47,56,70)') : (layer.bHighlight ? ('rgb(59,68,83)') : ('rgb(78,90,110)')),
      }" v-on:mouseenter="onmouseenter(layer)">
        <!-- 图层开关 -->
        <div :style="{
          width: `${page.height * 0.75}px`, height: `${page.height}px`, marginLeft: '3px',
          display: 'flex', justifyContent: 'center', alignItems: 'center'
        }">
          <img v-if="!layer.isOff" :src="'./resources/svg/layerOn.svg'"
            :style="{ width: `${page.height * 0.7}px`, height: `${page.height * 0.7}px` }"></img>
          <img v-if="layer.isOff" :src="'./resources/svg/layerOn.svg'"
            :style="{ width: `${page.height * 0.7}px`, height: `${page.height * 0.7}px` }"></img>
        </div>
        <!-- 图层是否冻结 -->
        <div :style="{
          width: `${page.height * 0.75}px`, height: `${page.height}px`,
          display: 'flex', justifyContent: 'center', alignItems: 'center'
        }">
          <img v-if="layer.isFrozen" :src="'./resources/svg/layfrz.svg'"
            :style="{ width: `${page.height * 0.7}px`, height: `${page.height * 0.7}px` }"></img>
          <img v-if="!layer.isFrozen" :src="'./resources/svg/layufrz.svg'"
            :style="{ width: `${page.height * 0.7}px`, height: `${page.height * 0.7}px` }"></img>
        </div>
        <!-- 图层是否锁定 -->
        <div :style="{
          width: `${page.height * 0.75}px`, height: `${page.height}px`,
          display: 'flex', justifyContent: 'center', alignItems: 'center'
        }">
          <img v-if="layer.isLocked" :src="'./resources/svg/laylck.svg'"
            :style="{ width: `${page.height * 0.7}px`, height: `${page.height * 0.7}px` }"></img>
          <img v-if="!layer.isLocked" :src="'./resources/svg/layulck.svg'"
            :style="{ width: `${page.height * 0.7}px`, height: `${page.height * 0.7}px` }"></img>
        </div>
        <!-- 图层颜色 -->
        <div :style="{
          width: `${page.height}px`, height: `${page.height}px`, marginRight: '3px',
          display: 'flex', justifyContent: 'center', alignItems: 'center'
        }">
          <div :style="{
            width: `${page.height * 0.6}px`, height: `${page.height * 0.6}px`,
            backgroundColor: `rgb(${layer.color.r},${layer.color.g},${layer.color.b})`, border: '1px solid rgb(0,0,0)'
          }" v-on:click="onclickColor(layer)">
          </div>
        </div>
        <!-- 图层名称 -->
        <div :style="{
          height: `${page.height}px`, fontSize: '70%', color: 'rgb(255,255,255)',
        }">
          <div :style="{
            width: `${page.width - (page.height * 4) - 10 + 150}px`, height: '100%',
            whiteSpace: 'nowrap', overflow: 'hidden', textOverflow: 'ellipsis',
            display: 'flex', alignItems: 'center',
          }">{{ layer.name }}</div>
        </div>
      </Row>
      <Row :style="{ height: '5px' }"></Row>
    </Scrollbar>

  </ModelBox>

</template>
