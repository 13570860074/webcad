<script setup lang="ts">
import { reactive } from 'vue';
import { gcadGlobal } from '@/gcadGlobal';
import { application } from '@/Application/FApplication';


/**
 * 页面参数
 */
const page: {
  width: number,
  height: number,
  left: number,
  top: number
} = reactive({
  width: gcadGlobal.uiLayoutData.canvasRight - gcadGlobal.uiLayoutData.canvasLeft,
  height: gcadGlobal.uiLayoutData.canvasBottom - gcadGlobal.uiLayoutData.canvasTop,
  left: gcadGlobal.uiLayoutData.workLeft,
  top: gcadGlobal.uiLayoutData.workTop,
});

/** 按下esc键 */
const escKeyDown = () => {
  let value: string = '{"altKey":false,"ctrlKey":false,"shiftKey":false,"keyCode":27,"key":"Escape"}';
  application.kernelManager().userInputManager().userInput('keydown', value);
};

// cursor: 'none'
</script>

<template>
  <div :style="{ position: 'relative' }" v-on:keydown="escKeyDown">
    <!-- 场景 -->
    <div id="scene" :style="{
      userSelect: 'none', width: `${page.width}px`, height: `${page.height}px`,
      position: 'absolute', top: `${0}px`, left: '0'
    }">
    </div>
    <!-- ESC按钮 -->
    <div :style="{
      zIndex: '2', position: 'absolute', width: '64px', height: '64px',
      marginLeft: `${30}px`, marginTop: `${30}px`, backgroundColor: 'rgb(145,145,145)',
      display: 'flex', justifyContent: 'center', alignItems: 'center',
      fontSize: '110%', color: 'rgb(255,255,255)', borderRadius: '10px',
      border: '1px solid rgb(0,0,0)',
    }" v-on:click="escKeyDown">ESC</div>
  </div>
</template>
