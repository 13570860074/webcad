<script setup lang="ts">
import { reactive } from 'vue';

const props = defineProps<{ autoHighlight?: boolean }>();

const page: {
  highlight: boolean,
} = reactive({
  highlight: false,
});
let autoHighlight: boolean = false;
if (props.autoHighlight != null) {
  autoHighlight = props.autoHighlight;
}


const onmouseenter = () => {
  if (autoHighlight === true) {
    page.highlight = true;
  }
};
const onmouseout = () => {
  if (autoHighlight === true) {
    page.highlight = false;
  }
};

</script>



<template>

  <div :style="{
    height: '20px', fontSize: '13px',
    backgroundColor: page.highlight ? 'rgb(224,238,249)' : 'rgb(253,253,253)',
    whiteSpace: 'pre', display: 'flex', justifyContent: 'center', alignItems: 'center'
  }" v-on:mouseenter="onmouseenter" v-on:mouseout="onmouseout">
    <slot></slot>
  </div>

</template>
