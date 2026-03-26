<script setup lang="ts">
import { reactive } from 'vue';
import Menubar from './components/Menubar.vue'
import WorkScene from './components/WorkScene.vue'
import { application } from './Application/FApplication'
import { EventState } from './EventManager/FEventManager'
import { gcadGlobal, IUiLayoutData } from './gcadGlobal';
import Toolbar from './components/Toolbar.vue';

const page: {
  width: number,
  height: number,
  left: number,
  top: number
} = reactive({
  width: gcadGlobal.uiLayoutData.workRight - gcadGlobal.uiLayoutData.workLeft,
  height: gcadGlobal.uiLayoutData.workBottom - gcadGlobal.uiLayoutData.workTop,
  left: gcadGlobal.uiLayoutData.workLeft,
  top: gcadGlobal.uiLayoutData.workTop,
});


/** 监听更新布局数据事件 */
application.eventManager().on(EventState.kUpdateLayoutData, (layoutData: IUiLayoutData) => {
  page.width = gcadGlobal.uiLayoutData.workRight - gcadGlobal.uiLayoutData.workLeft;
  page.height = gcadGlobal.uiLayoutData.workBottom - gcadGlobal.uiLayoutData.workTop;
  page.left = layoutData.workLeft;
  page.top = layoutData.workTop;
});

</script>



<template>
  <div id="gcad" :style="{
    zIndex: '2', position: 'absolute',
    width: `${page.width}px`, height: `${page.height}px`, left: `${page.left}px`, top: `${page.top}px`,
    backgroundColor: 'rgb(255,255,255)', display: 'flex', flexDirection: 'column',
  }" v-on:contextmenu="(e) => { e.preventDefault() }">
    <Menubar />
    <Toolbar />
    <WorkScene />
  </div>
</template>
