<script setup lang="ts">
import { reactive } from 'vue';

const props = defineProps<{ span?: number }>();

const page: {
  span: number,
} = reactive({
  span: 100
});

if (props.span != null) {
  page.span = props.span;
}

</script>



<template>

  <div :style="{ width: `${page.span}%`, height: '100%', display: 'flex', flexDirection: 'column' }">
    <slot></slot>
  </div>

</template>
