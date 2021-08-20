function random (min, max) {
  min = Math.ceil(min)
  max = Math.floor(max)
  return Math.floor(Math.random() * (max - min + 1)) + min
}

function camelCase (string) {
  let newStr = string.replace(/'/g,'').replace(/-/g,'').split(' ')
  for (let i = 0; i < newStr.length; i++) {
    newStr[i] = newStr[i].charAt(0).toUpperCase() + newStr[i].substring(1)
  }
  newStr = newStr.join('')
  newStr = newStr.charAt(0).toLowerCase() + newStr.substring(1)
  return newStr
}

function median(numbers) {
  const sorted = numbers.slice().sort((a, b) => a - b);
  const middle = Math.floor(sorted.length / 2);

  if (sorted.length % 2 === 0) {
      return (sorted[middle - 1] + sorted[middle]) / 2;
  }

  return sorted[middle];
}