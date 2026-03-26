<script setup lang="ts">
import Arx from '@/kernel/arxHeaders';
import { reactive } from 'vue';
import Row from './Row.vue';
import { gcadGlobal_uiLayoutData } from '@/gcadGlobal';
import Column from './Column.vue';
import { FInterface } from '@/Core/FInterface';


/**
 * 页面参数
 */
const page: {

} = reactive({

});
const width: number = gcadGlobal_uiLayoutData().workRight - gcadGlobal_uiLayoutData().workLeft;
const menubarHeight: number = gcadGlobal_uiLayoutData().menubarHeight;



const onclickOpenFile = async () => {
  let files: FileList = await FInterface.selectLocalFiles();
  if (files.length <= 0) {
    return;
  }

  let data: string = <string>await FInterface.loadFileText(files[0]);
    Arx.acdbHostApplicationServices().load_dxf_data(data);

}



</script>

<template>
  <Row :style="{ height: `${menubarHeight}px` }">

    <!-- 后退图标 -->
    <Column :style="{
      width: `${menubarHeight}px`, justifyContent: 'center', alignItems: 'center'
    }">
      <img :src="'./resources/svg/arrow_left.svg'"
        :style="{ width: `${menubarHeight * 0.35}px`, height: `${menubarHeight * 0.35}px` }"></img>
    </Column>
    <!-- 标题 -->
    <Column :style="{ width: `${width * 0.5 - menubarHeight}px` }">
      <Row :style="{ height: '100%', alignItems: 'center' }">
        <div :style="{ fontSize: '95%' }">这个是图纸标题</div>
        <img :src="'./resources/svg/editName.svg'" :style="{
          marginLeft: '10px', width: `${menubarHeight * 0.35}px`, height: `${menubarHeight * 0.35}px`
        }"></img>
      </Row>
    </Column>
    <!-- 右边按钮 -->
    <Column :style="{ width: `${width * 0.5}px`, flexDirection: '', justifyContent: 'flex-end', alignItems: 'center' }">
      <!-- undo -->
      <img :src="'./resources/svg/undo.svg'" :style="{
        marginRight: `${menubarHeight / 2}px`, width: `${menubarHeight * 0.5}px`, height: `${menubarHeight * 0.5}px`
      }"></img>
      <!-- redo -->
      <img :src="'./resources/svg/redo.svg'" :style="{
        marginRight: `${menubarHeight / 3}px`, width: `${menubarHeight * 0.5}px`, height: `${menubarHeight * 0.5}px`
      }"></img>
      <div :style="{
        width: '2px', height: '50%', marginTop: '25%', marginBottom: '25%', marginRight: `${menubarHeight / 3}px`,
        backgroundColor: 'rgb(155,155,155)'
      }"></div>
      <!-- 打开文件 -->
      <img :src="'./resources/svg/openFile.svg'" :style="{
        marginRight: `${menubarHeight / 2}px`, width: `${menubarHeight * 0.5}px`, height: `${menubarHeight * 0.5}px`
      }" v-on:click="onclickOpenFile"></img>
      <!-- 布局 -->
      <img :src="'./resources/svg/layout.svg'" :style="{
        marginRight: `${menubarHeight / 2}px`, width: `${menubarHeight * 0.5}px`, height: `${menubarHeight * 0.5}px`
      }"></img>
      <!-- 展开 -->
      <img :src="'./resources/svg/expand.svg'" :style="{
        marginRight: `${menubarHeight / 2}px`, width: `${menubarHeight * 0.5}px`, height: `${menubarHeight * 0.5}px`
      }"></img>
    </Column>

  </Row>

</template>
